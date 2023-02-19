#ifndef ROOT_OBSERVABLE_H
#define ROOT_OBSERVABLE_H

#include "CanSubscribe.h"
#include "Subscriber.h"
#include "Types.h"

class CanTeardown;
class Subscription;

template <typename T>
class Subject;

template <typename U, typename T>
class Operator;

template <typename T>
class Observable : public CanSubscribe<T>, public Object
{
  OBJECT(Observable<T>)

  template <typename W>
  friend class Subject;

protected:
  std::function<CanTeardown *(Subscriber<T> *)> __subscribe;

public:
  Observable() = default;
  Observable(
      const std::function<CanTeardown *(Subscriber<T> *)> &subscribe);

  virtual ~Observable() = default;

public:
  virtual Subscription *subscribe(Observer<T> *subscriber);
  virtual Subscription *subscribe(SimpleTypedObserver<T> *subscriber);

  template <typename R>
  Observable<R> *liftRoot(
      Operator<T, R> *liftOperator,
      const std::function<Observable<R> *(Observable<T> *,
                                               Operator<T, R> *)> &transform);

protected:
  virtual CanTeardown *_trySubscribe(Subscriber<T> *subscriber);
  virtual CanTeardown *_subscribe(Subscriber<T> *subscriber);
};

template <typename T>
static Observable<T> *emptyRootObservable();

template <typename T>
inline Observable<T> *emptyRootObservable() {
  return new Observable<T>([](Subscriber<T> *subscriber) -> CanTeardown * {
    subscriber->complete();
    return cleanup([]() {});
  });
}

#include "Observable.tpp"

#endif