#ifndef SIMPLE_TYPED_OBSERVER_H
#define SIMPLE_TYPED_OBSERVER_H

#include <functional>
#include <optional>

#include <TkArch/Meta.h>

#include "SimpleObserver.h"

template <typename T>
class SimpleTypedObserver : public SimpleObserver
{
  OBJECT(SimpleTypedObserver<T>)

protected:
  std::optional<NextFn<T>> m_next_T;

public:
  // Create an observer that does nothing
  SimpleTypedObserver() = default;
  // Create an observer that only calls the next function
  SimpleTypedObserver(const std::optional<NextFn<T>> &next);
  // Create an observer that only calls the error function
  SimpleTypedObserver(const std::optional<ErrorFn> &next);
  // Create an observer that only calls the complete function
  SimpleTypedObserver(const std::optional<CompleteFn> &next);
  // Create an observer that calls the next and error functions
  SimpleTypedObserver(const std::optional<NextFn<T>> &next,
                      const std::optional<ErrorFn> &error);
  // Create an observer that calls the next, error and complete functions
  SimpleTypedObserver(const std::optional<NextFn<T>> &next,
                      const std::optional<ErrorFn> &error,
                      const std::optional<CompleteFn> &complete);

public:
  virtual void next(const T &value);
};

#include "SimpleTypedObserver.tpp"

#endif // SIMPLE_TYPED_OBSERVER_H