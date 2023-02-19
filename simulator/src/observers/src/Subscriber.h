#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <TkArch/Meta.h>

#include "Observer.h"
#include "Subscription.h"

template <typename T, typename U = T>
class Subscriber : public Subscription, public Observer<T>
{
  OBJECT(Subscriber<T, U>)

protected:
  bool _isStopped = false;
  Observer<U> *_destination;

protected:
  Subscriber();
  Subscriber(Subscriber<U> *destination);
  Subscriber(Observer<U> *destination);

public:
  virtual ~Subscriber();

public:
  void next(const T &value) override;
  void error( std::exception &error) override;
  void complete() override;

protected:
  virtual void _next(const T &value);
  virtual void _error( std::exception &error);
  virtual void _complete();
};

#include "Subscriber.tpp"

#endif // SUBSCRIBER_H