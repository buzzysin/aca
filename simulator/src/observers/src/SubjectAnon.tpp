#ifndef SUBJECT_ANON_TPP
#define SUBJECT_ANON_TPP

#include "SubjectAnon.h"

template <typename T>
inline SubjectAnon<T>::SubjectAnon(Observer<T> *destination,
                                   LiftedObservable<T> *source)
    : Subject<T>(), _destination(destination), _source(source) {}

template <typename T>
void SubjectAnon<T>::next(const T &value) {
  if (this->_destination == nullptr)
    return;

  this->_destination->next(value);
}

template <typename T>
inline void SubjectAnon<T>::error(const std::exception &error) {
  if (this->_destination == nullptr)
    return;

  this->_destination->error(error);
}

template <typename T>
inline void SubjectAnon<T>::complete() {
  if (this->_destination == nullptr)
    return;

  this->_destination->complete();
}

template <typename T>
inline Subscription *SubjectAnon<T>::_subscribe(Subscriber<T> *subscriber) {
  if (this->_source == nullptr)
    return emptySubscription();

  return this->_source->subscribe(subscriber);
}

#endif