#ifndef BEHAVIOUR_SUBJECT_TPP
#define BEHAVIOUR_SUBJECT_TPP

#include "BehaviourSubject.h"

template <typename T>
BehaviourSubject<T>::BehaviourSubject(const T &value)
    : Subject<T>(), _value(value) {}

template <typename T> void BehaviourSubject<T>::next(const T &value) {
  _value = value;
  Subject<T>::next(value);
}

template <typename T> T BehaviourSubject<T>::value() { return _value; }

template <typename T> T BehaviourSubject<T>::getValue() {
  if (Subject<T>::_hasError) {
    throw Subject<T>::_thrownError;
  }

  Subject<T>::_throwIfClosed();

  return _value;
}

template <typename T>
Subscription *BehaviourSubject<T>::_subscribe(Subscriber<T> *subscriber) {
  Subscription *subscription = Subject<T>::_subscribe(subscriber);

  if (!subscription->closed())
    subscriber->next(_value);

  return subscription;
}

#endif // BEHAVIOUR_SUBJECT_H
