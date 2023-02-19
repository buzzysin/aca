#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "CanSubscribe.h"
#include "ErrorContext.h"
#include "Observable.h"
#include "Subscriber.h"
#include "Types.h"

#include <TkArch/Meta.h>
#include <functional>

class CanTeardown;

class Subscription;

template <typename T>
class Subject;

template <typename T, typename S = T>
class LiftedObservable : public Observable<T>
{
  OBJECT(LiftedObservable<T>)

  template <typename W>
  friend class Subject;

  template <typename U, typename V>
  friend class LiftedObservable;

protected:
  Observable<S> *_source;
  Operator<S, T> *_operator;

public:
  LiftedObservable() = default;
  LiftedObservable(
      const std::function<CanTeardown *(Subscriber<T> *)> &subscribe);

  virtual ~LiftedObservable() = default;

public:
  Subscription *subscribe(Observer<S> *subscriber);
  Subscription *subscribe(SimpleTypedObserver<S> *subscriber);

  template <typename R>
  LiftedObservable<R, T> *lift(Operator<T, R> *liftOperator);

  template <typename R>
  LiftedObservable<R, T> *
  pipe(const UnaryFunction<Observable<T> *, Observable<R> *> &opFn);

protected:
  virtual CanTeardown *_trySubscribe(Subscriber<S> *subscriber);
  virtual CanTeardown *_subscribe(Subscriber<S> *subscriber);
};

template <typename T>
static LiftedObservable<T> *emptyObservable();

template <typename T>
inline LiftedObservable<T> *emptyObservable() {
  return new LiftedObservable<T>(
      [](Subscriber<T> *subscriber) -> CanTeardown * {
        subscriber->complete();
        return cleanup([]() {});
      });
}

#include "LiftedObservable.tpp"

#endif // OBSERVER_H