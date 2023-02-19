#ifndef OBSERVER_H
#define OBSERVER_H

#include <TkArch/Meta.h>

#include "SimpleTypedObserver.h"

template <typename T>
class Observer : public SimpleTypedObserver<T>
{
  OBJECT(Observer<T>)

protected:
  std::optional<NextFn<T>> m_next;

public:
  // Create an empty observer
  Observer();
  // Create an observer that only calls the next callback
  Observer(const NextFn<T> &next);
  // Create an observer that only calls the error callback
  Observer(const ErrorFn &error);
  // Create an observer that only calls the complete callback
  Observer(const CompleteFn &complete);
  // Create an observer with the given callbacks
  Observer(const NextFn<T> &next, const ErrorFn &error);
  // Create an observer with the given callbacks
  Observer(const NextFn<T> &next, const ErrorFn &error,
           const CompleteFn &complete);
  virtual ~Observer();

  // public:
  //   virtual void next(const T &value);
};

template <typename T>
static Observer<T> *emptyObserver();

#include "Observer.tpp"

// Shortcut to create an observer
#define obs Observer

#endif