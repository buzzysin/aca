#ifndef SIMPLE_OBSERVER_H
#define SIMPLE_OBSERVER_H

#include <functional>
#include <iostream>
#include <optional>

#include <TkArch/Meta.h>

using VoidFn = std::function<void()>;

template <typename T>
using NextFn = std::function<void(const T &)>;

using AnonNextFn = NextFn<void *>;
using ErrorFn = std::function<void(std::exception &)>;
using CompleteFn = VoidFn;

class SimpleObserver : public Object
{
  OBJECT(SimpleObserver)

protected:
  std::optional<AnonNextFn> m_next;
  std::optional<ErrorFn> m_error;
  std::optional<CompleteFn> m_complete;

public:
  //  Createa an observer that does nothing
  SimpleObserver() = default;
  // Create an observer that only calls the next function
  SimpleObserver(const std::optional<AnonNextFn> &next);
  // Create an observer that only calls the error function
  SimpleObserver(const std::optional<ErrorFn> &next);
  // Create an observer that only calls the complete function
  SimpleObserver(const std::optional<CompleteFn> &next);
  // Create an observer that calls the next and error functions
  SimpleObserver(const std::optional<AnonNextFn> &next,
                 const std::optional<ErrorFn> &error);
  // Create an observer that calls the next, error and complete functions
  SimpleObserver(const std::optional<AnonNextFn> &next,
                 const std::optional<ErrorFn> &error,
                 const std::optional<CompleteFn> &complete);

public:
  virtual void next(void *value);
  virtual void error( std::exception &e);
  virtual void complete();
};

#include "SimpleObserver.tpp"

#endif // SIMPLE_OBSERVER_H