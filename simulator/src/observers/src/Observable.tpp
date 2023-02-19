#ifndef OBSERVABLE_TPP
#define OBSERVABLE_TPP

#include "Observable.h"

#include "ErrorContext.h"
#include "SubscriberSafe.h"

template <typename T>
inline Observable<T>::Observable(
    const std::function<CanTeardown *(Subscriber<T> *)> &subscribe)
    : CanSubscribe<T>() {
  tkDebug() << "Observable::Observable()";

  __subscribe = subscribe;
}

template <typename T>
inline Subscription *Observable<T>::subscribe(Observer<T> *subscriber) {
  tkDebug() << "Observable::subscribe()";

  SubscriberSafe<T> *safeSubscriber = new SubscriberSafe<T>(subscriber);

  errorContext([&]() {
    safeSubscriber->add(new Subscription(_subscribe(safeSubscriber)));
  });

  return safeSubscriber;
}

template <typename T>
inline Subscription *
Observable<T>::subscribe(SimpleTypedObserver<T> *subscriber) {
  tkDebug() << "Observable::subscribe()";

  SubscriberSafe<T> *safeSubscriber = new SubscriberSafe<T>(subscriber);

  errorContext([&]() {
    safeSubscriber->add(new Subscription(_subscribe(safeSubscriber)));
  });

  return safeSubscriber;
}

template <typename T>
template <typename R>
inline Observable<R> *Observable<T>::liftRoot(
    Operator<T, R> *liftOperator,
    const std::function<Observable<R> *(Observable<T> *, Operator<T, R> *)>
        &transform) {
  tkDebug() << "Observable::lift()";

  Observable<R> *liftedObservable = transform(this, liftOperator);

  return liftedObservable;
}

template <typename T>
inline CanTeardown *Observable<T>::_trySubscribe(Subscriber<T> *subscriber) {
  tkDebug() << "Observable::_trySubscribe()";

  try {
    return _subscribe(subscriber);
  } catch (std::exception &error) {
    subscriber->error(error);
    return cleanup([]() {});
  }
}

template <typename T>
inline CanTeardown *Observable<T>::_subscribe(Subscriber<T> *subscriber) {
  tkDebug() << "Observable::_subscribe()";

  if (__subscribe) {
    return __subscribe(subscriber);
  } else {
    return cleanup([]() {});
  }
}

#endif // OBSERVABLE_TPP