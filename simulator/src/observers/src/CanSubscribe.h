#ifndef CAN_SUBSCRIBE_H
#define CAN_SUBSCRIBE_H

#include <TkArch/Meta.h>

class CanUnsubscribe;

template <typename T>
class SimpleTypedObserver;

template <typename T>
class CanSubscribe
{
  OBJECT(CanSubscribe<T>)

public:
  virtual ~CanSubscribe() = default;

  virtual CanUnsubscribe *subscribe(SimpleTypedObserver<T> *observer) = 0;
};

#include "CanSubscribe.tpp"

#endif // CAN_SUBSCRIBE_H