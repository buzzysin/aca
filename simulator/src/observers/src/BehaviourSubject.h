#ifndef BEHAVIOUR_SUBJECT_H
#define BEHAVIOUR_SUBJECT_H

#include <TkArch/Meta.h>

#include "Subject.h"
#include "Subscriber.h"
#include "Subscription.h"

template <typename T> class BehaviourSubject : public Subject<T> {
  OBJECT(BehaviourSubject<T>)

protected:
  T _value;

public:
  BehaviourSubject(const T &value);

public:
  void next(const T &value);

  T value();
  T getValue();

protected:
  Subscription *_subscribe(Subscriber<T> *observer);
};

#include "BehaviourSubject.tpp"

#endif // BEHAVIOUR_SUBJECT_H