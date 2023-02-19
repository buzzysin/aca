#ifndef OPERATOR_SUBSCRIBER_H
#define OPERATOR_SUBSCRIBER_H

#include "Subscriber.h"

template <typename T, typename SourceT = T>
class OperatorSubscriber : public Subscriber<T>
{
protected:
  std::optional<NextFn<T>> _nextFn;
  std::optional<CompleteFn> _completeFn;
  std::optional<ErrorFn> _errorFn;
  std::optional<VoidFn> _teardownFn;
  std::optional<std::function<bool()>> _shouldUnsubscribe;

public:
  OperatorSubscriber(Subscriber<SourceT> *destination);
  OperatorSubscriber(Subscriber<SourceT> *destination, const NextFn<T> &nextFn);
  OperatorSubscriber(Subscriber<SourceT> *destination, const NextFn<T> &nextFn,
                     const CompleteFn &completeFn);
  OperatorSubscriber(Subscriber<SourceT> *destination, const NextFn<T> &nextFn,
                     const CompleteFn &completeFn, const ErrorFn &errorFn);
  OperatorSubscriber(Subscriber<SourceT> *destination, const NextFn<T> &nextFn,
                     const CompleteFn &completeFn, const ErrorFn &errorFn,
                     const VoidFn &teardownFn);
  OperatorSubscriber(Subscriber<SourceT> *destination, const NextFn<T> &nextFn,
                     const CompleteFn &completeFn, const ErrorFn &errorFn,
                     const VoidFn &teardownFn,
                     std::function<bool()> &shouldUnsubscribe);

public:
  void _next(const T &value) override;
  void _error( std::exception &error) override;
  void _complete() override;

  void unsubscribe() override;
};

template <typename T, typename SourceT, typename V = T, typename W = SourceT>
static OperatorSubscriber<T, SourceT> *
createOperatorSubscriber(Subscriber<SourceT> *destination) {
  return new OperatorSubscriber<T, SourceT>(destination);
}

template <typename T, typename SourceT, typename V = T, typename W = SourceT>
static OperatorSubscriber<T, SourceT> *
createOperatorSubscriber(Subscriber<SourceT> *destination,
                         const NextFn<T> &nextFn) {
  return new OperatorSubscriber<T, SourceT>(destination, nextFn);
}

template <typename T, typename SourceT, typename V = T, typename W = SourceT>
static OperatorSubscriber<T, SourceT> *
createOperatorSubscriber(Subscriber<SourceT> *destination,
                         const NextFn<T> &nextFn,
                         const CompleteFn &completeFn) {
  return new OperatorSubscriber<T, SourceT>(destination, nextFn, completeFn);
}

template <typename T, typename SourceT, typename V = T, typename W = SourceT>
static OperatorSubscriber<T, SourceT> *
createOperatorSubscriber(Subscriber<SourceT> *destination,
                         const NextFn<T> &nextFn, const CompleteFn &completeFn,
                         const ErrorFn &errorFn) {
  return new OperatorSubscriber<T, SourceT>(destination, nextFn, completeFn,
                                            errorFn);
}

template <typename T, typename SourceT, typename V = T, typename W = SourceT>
static OperatorSubscriber<T, SourceT> *
createOperatorSubscriber(Subscriber<SourceT> *destination,
                         const NextFn<T> &nextFn, const CompleteFn &completeFn,
                         const ErrorFn &errorFn, const VoidFn &teardownFn) {
  return new OperatorSubscriber<T, SourceT>(destination, nextFn, completeFn,
                                            errorFn, teardownFn);
}

template <typename T, typename SourceT, typename V = T, typename W = SourceT>
static OperatorSubscriber<T, SourceT> *
createOperatorSubscriber(Subscriber<SourceT> *destination,
                         const NextFn<T> &nextFn, const CompleteFn &completeFn,
                         const ErrorFn &errorFn, const VoidFn &teardownFn,
                         std::function<bool()> &shouldUnsubscribe) {
  return new OperatorSubscriber<T, SourceT>(
      destination, nextFn, completeFn, errorFn, teardownFn, shouldUnsubscribe);
}

#include "OperatorSubscriber.tpp"

#endif