#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>

#include <TkArch/Meta.h>

#include "LiftedObservable.h"
#include "Subscriber.h"

class CanTeardown;

template <typename T>
class Observer;

template <typename T>
class Subject : public LiftedObservable<T>, public LikeSubscription
{
  OBJECT(Subject<T>)

  template <typename U, typename V>
  friend class LiftedObservable;

protected:
  std::vector<Observer<T> *> _currentObservers;
  std::vector<Observer<T> *> _observers;

  bool _closed    = false;
  bool _isStopped = false;
  bool _hasError  = false;
  std::exception _thrownError;

public:
  Subject() = default;

public:
  bool closed() const override;

  void next(const T &value);
  void error(const std::exception &error);
  void complete();

  void unsubscribe() override;

  bool observed();

  template <typename U>
  LiftedObservable<U> *asObservable();

protected:
  CanTeardown *_trySubscribe(Subscriber<T> *subscriber) override;
  Subscription *_subscribe(Subscriber<T> *subscriber) override;

  template <typename U>
  Subscription *_innerSubscribe(Subscriber<U> *subscriber);

  void _checkFinalizedStatuses(Subscriber<T> *subscriber);
  void _throwIfClosed();
};

#include "Subject.tpp"

#endif