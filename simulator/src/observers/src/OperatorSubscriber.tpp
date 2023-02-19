#ifndef OPERATOR_SUBSCRIBER_TPP
#define OPERATOR_SUBSCRIBER_TPP

#include "OperatorSubscriber.h"

template <typename T, typename SourceT>
OperatorSubscriber<T, SourceT>::OperatorSubscriber(
    Subscriber<SourceT> *destination)
    : Subscriber<T, SourceT>(destination) {}

template <typename T, typename SourceT>
OperatorSubscriber<T, SourceT>::OperatorSubscriber(
    Subscriber<SourceT> *destination, const NextFn<T> &nextFn)
    : Subscriber<T, SourceT>(destination), _nextFn(nextFn) {}

template <typename T, typename SourceT>
OperatorSubscriber<T, SourceT>::OperatorSubscriber(
    Subscriber<SourceT> *destination, const NextFn<T> &nextFn,
    const CompleteFn &completeFn)
    : Subscriber<T, SourceT>(destination), _nextFn(nextFn),
      _completeFn(completeFn) {}

template <typename T, typename SourceT>
OperatorSubscriber<T, SourceT>::OperatorSubscriber(
    Subscriber<SourceT> *destination, const NextFn<T> &nextFn,
    const CompleteFn &completeFn, const ErrorFn &errorFn)
    : Subscriber<T, SourceT>(destination), _nextFn(nextFn),
      _completeFn(completeFn), _errorFn(errorFn) {}

template <typename T, typename SourceT>
OperatorSubscriber<T, SourceT>::OperatorSubscriber(
    Subscriber<SourceT> *destination, const NextFn<T> &nextFn,
    const CompleteFn &completeFn, const ErrorFn &errorFn,
    const VoidFn &teardownFn)
    : Subscriber<T, SourceT>(destination), _nextFn(nextFn),
      _completeFn(completeFn), _errorFn(errorFn), _teardownFn(teardownFn) {}

template <typename T, typename SourceT>
OperatorSubscriber<T, SourceT>::OperatorSubscriber(
    Subscriber<SourceT> *destination, const NextFn<T> &nextFn,
    const CompleteFn &completeFn, const ErrorFn &errorFn,
    const VoidFn &teardownFn, std::function<bool()> &shouldUnsubscribe)
    : Subscriber<T, SourceT>(destination), _nextFn(nextFn),
      _completeFn(completeFn), _errorFn(errorFn), _teardownFn(teardownFn),
      _shouldUnsubscribe(shouldUnsubscribe) {}

template <typename T, typename SourceT>
void OperatorSubscriber<T, SourceT>::_next(const T &value) {
  if (_nextFn.has_value()) {
    try {
      _nextFn.value()(value);
    } catch (std::exception &e) {
      Subscriber<T, SourceT>::_error(e);
    }
  } else {
    Subscriber<T, SourceT>::_next(value);
  }
}

template <typename T, typename SourceT>
void OperatorSubscriber<T, SourceT>::_error( std::exception &error) {
  if (_errorFn.has_value()) {
    try {
      _errorFn.value()(error);
    } catch (std::exception &e) {
      Subscriber<T, SourceT>::_error(e);

      unsubscribe();
    }
  } else {
    Subscriber<T, SourceT>::_error(error);
  }
}

template <typename T, typename SourceT>
void OperatorSubscriber<T, SourceT>::_complete() {
  if (_completeFn.has_value()) {
    try {
      _completeFn.value()();
    } catch (std::exception &e) {
      Subscriber<T, SourceT>::_error(e);

      unsubscribe();
    }
  } else {
    Subscriber<T, SourceT>::_complete();
  }
}

template <typename T, typename SourceT>
void OperatorSubscriber<T, SourceT>::unsubscribe() {
  if (!_shouldUnsubscribe.has_value() || _shouldUnsubscribe.value()()) {
    Subscriber<T, SourceT>::unsubscribe();

    if (this->closed() && _teardownFn.has_value()) {
      _teardownFn.value()();
    }
  }
}

#endif