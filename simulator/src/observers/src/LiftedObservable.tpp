#ifndef LIFTED_OBSERVABLE_TPP
#define LIFTED_OBSERVABLE_TPP

#include "LiftedObservable.h"

#include "ErrorContext.h"
#include "SubscriberSafe.h"

template <typename T, typename S>
inline LiftedObservable<T, S>::LiftedObservable(
    const std::function<CanTeardown *(Subscriber<T> *)> &subscribe)
    : Observable<T>(subscribe), CanSubscribe<T>() {
  tkDebug() << "LiftedObservable::LiftedObservable()";
}

template <typename T, typename S>
inline Subscription *
LiftedObservable<T, S>::subscribe(SimpleTypedObserver<S> *subscriber) {
  tkDebug() << "LiftedObservable::subscribe()";

  SubscriberSafe<T> *safeSubscriber = new SubscriberSafe<S>(subscriber);

  errorContext([&]() {
    // TODO: operator support?
    safeSubscriber->add(new Subscription(
        _source ? _subscribe(safeSubscriber) : _trySubscribe(safeSubscriber)));
  });

  return safeSubscriber;
}

template <typename T, typename S>
inline Subscription *
LiftedObservable<T, S>::subscribe(Observer<S> *subscriber) {
  tkDebug() << "LiftedObservable::subscribe()";

  SubscriberSafe<T> *safeSubscriber = new SubscriberSafe<S>(subscriber);

  errorContext([&]() {
    // TODO: operator support?
    safeSubscriber->add(new Subscription(
        _source ? _subscribe(safeSubscriber) : _trySubscribe(safeSubscriber)));
  });

  return safeSubscriber;
}

template <typename T, typename S>
template <typename R>
inline LiftedObservable<R, T> *
LiftedObservable<T, S>::lift(Operator<T, R> *liftOperator) {
  tkDebug() << "LiftedObservable::lift()";

  Observable<R> *lifted =
      liftRoot<R>(liftOperator, [](Observable<T> *o, Operator<T, R> *op) {
        LiftedObservable<R, T> *l = new LiftedObservable<R, T>();
        l->_source                = o;
        l->_operator              = op;
        return l;
      });

  return reinterpret_cast<LiftedObservable<R, T> *>(lifted);
}

template <typename T, typename S>
template <typename R>
inline LiftedObservable<R, T> *
LiftedObservable<T, S>::pipe(const UnaryFunction<Observable<T>*, Observable<R>*> &opFn) {
  tkDebug() << "LiftedObservable::pipe()";
  
  Observable<R> *lifted = opFn(this);

  return nullptr;
}

template <typename T, typename S>
inline CanTeardown *
LiftedObservable<T, S>::_trySubscribe(Subscriber<S> *subscriber) {
  tkDebug() << "LiftedObservable::_trySubscribe()";

  try {
    return _subscribe(subscriber);
  } catch (std::exception &error) {
    subscriber->error(error);
  }

  return CanTeardown::fromFunction([]() {
    tkDebug()
        << "LiftedObservable::_trySubscribe() -> CanTeardown::fromFunction()";
  });
}

template <typename T, typename S>
inline CanTeardown *
LiftedObservable<T, S>::_subscribe(Subscriber<S> *subscriber) {
  tkDebug() << "LiftedObservable::_subscribe()";

  /* if (this->__subscribe) {
    return this->__subscribe(subscriber);
  } else  */
  if (this->_source) {
    return this->_source->subscribe(subscriber);
  }
  // If neither have been set, return a teardown function that does nothing
  else {
    return CanTeardown::fromFunction([]() {
      tkDebug()
          << "LiftedObservable::_subscribe() -> CanTeardown::fromFunction()";
    });
  }
}

#endif // OBSERVER_H