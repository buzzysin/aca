#ifndef OBSERVER_CONSUMER_H
#define OBSERVER_CONSUMER_H

#include <TkArch/Meta.h>

#include "Observer.h"
#include "SimpleTypedObserver.h"

template <typename T> class ObserverConsumer : public Observer<T> {
  OBJECT(ObserverConsumer<T>)

  template <typename U> friend class SubscriberSafe;

protected:
  SimpleTypedObserver<T> *_partialObserver;

protected:
  ObserverConsumer(SimpleTypedObserver<T> *partialObserver);

public:
  void next(const T &value) override;
  void error( std::exception &error) override;
  void complete() override;
};

#include "ObserverConsumer.tpp"

#endif // OBSERVER_CONSUMER_H