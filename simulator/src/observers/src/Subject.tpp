#ifndef SUBJECT_TPP
#define SUBJECT_TPP

#include "Subject.h"

#include "ErrorContext.h"
#include "LiftedObservable.h"


template <typename T>
bool Subject<T>::closed() const {
  return _closed;
}

template <typename T>
void Subject<T>::next(const T &value) {
  errorContext([this, &value]() {
    _throwIfClosed();

    if (_isStopped)
      return;

    if (_currentObservers.empty()) {

      for (auto observer : _observers) {
        _currentObservers.push_back(observer);
      }
    }

    for (auto observer : _currentObservers) {
      observer->next(value);
    }
  });
}

template <typename T>
void Subject<T>::error(const std::exception &error) {
  errorContext([this, &error]() {
    _throwIfClosed();

    if (_isStopped)
      return;

    _isStopped   = true;
    _hasError    = true;
    _thrownError = error;

    while (!_observers.empty()) {
      _observers.front()->error(error);
      _observers.erase(_observers.begin());
    }
  });
}

template <typename T>
void Subject<T>::complete() {
  errorContext([this]() {
    _throwIfClosed();

    if (_isStopped)
      return;

    _isStopped = true;

    while (!_observers.empty()) {
      _observers.front()->complete();

      _observers.erase(_observers.begin());
    }
  });
}

template <typename T>
void Subject<T>::unsubscribe() {
  _closed    = true;
  _isStopped = true;
  _observers.clear();
  _currentObservers.clear();
}

template <typename T>
bool Subject<T>::observed() {
  return !_observers.empty();
}

template <typename T>
template <typename U>
LiftedObservable<U> *Subject<T>::asObservable() {
  auto newObservable     = new LiftedObservable<U>();
  newObservable->_source = reinterpret_cast<LiftedObservable<U> *>(this);
  return newObservable;
}

template <typename T>
CanTeardown *Subject<T>::_trySubscribe(Subscriber<T> *subscriber) {
  _throwIfClosed();
  return LiftedObservable<T>::_trySubscribe(subscriber);
}

template <typename T>
Subscription *Subject<T>::_subscribe(Subscriber<T> *subscriber) {
  _throwIfClosed();
  _checkFinalizedStatuses(subscriber);
  return _innerSubscribe(subscriber);
}

template <typename T>
template <typename U>
Subscription *Subject<T>::_innerSubscribe(Subscriber<U> *subscriber) {
  if (_hasError || _isStopped) {
    return emptySubscription();
  }

  _currentObservers.clear();

  _observers.push_back(subscriber);

  return new Subscription(CanTeardown::fromFunction([this, subscriber]() {
    _currentObservers.clear();
    // _observers.remove(subscriber);
    _observers.erase(
        std::remove(_observers.begin(), _observers.end(), subscriber),
        _observers.end());
  }));
}

template <typename T>
void Subject<T>::_checkFinalizedStatuses(Subscriber<T> *subscriber) {
  if (_hasError) {
    subscriber->error(_thrownError);
  } else if (_isStopped) {
    subscriber->complete();
  }
}

template <typename T>
void Subject<T>::_throwIfClosed() {
  if (_closed) {
    throw std::runtime_error("Subject has been closed and is no longer usable");
  }
}

#endif