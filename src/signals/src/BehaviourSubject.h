#ifndef BEHAVIOUR_SUBJECT_H
#define BEHAVIOUR_SUBJECT_H

#include "Subject.h"

template <typename T>
class behaviour_subject : public subject<T>
{
protected:
  using subject<T>::_errored;
  using subject<T>::_error;
  using subject<T>::_throw_if_closed;

public:
  using subject<T>::subscribe;

protected:
  T _value;

public:
  behaviour_subject(T value) : subject<T>(), _value(value) {}

  virtual ~behaviour_subject() {}

  virtual void next(T value) {
    _value = value;
    subject<T>::next(value);
  }

  virtual sp<subscription> subscribe(sp<observer<T>> obs) override {
    auto subscription = subject<T>::subscribe(obs);
    if (!subscription->is_closed())
      obs->next(_value);

    return subscription;
  }

  T value() const {
    if (_errored)
      throw _error;

    _throw_if_closed();

    return _value;
  }
};

#endif