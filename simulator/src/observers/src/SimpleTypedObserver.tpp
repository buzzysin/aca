#ifndef SIMPLE_TYPED_OBSERVER_TPP
#define SIMPLE_TYPED_OBSERVER_TPP

#include "SimpleTypedObserver.h"

#include <TkArch/Meta.h>


template <typename T>
inline SimpleTypedObserver<T>::SimpleTypedObserver(
    const std::optional<NextFn<T>> &_next)
    : SimpleObserver() {
  m_next_T = _next;
  tkDebug() << "SimpleTypedObserver::SimpleTypedObserver()";
}

template <typename T>
inline SimpleTypedObserver<T>::SimpleTypedObserver(
  const std::optional<ErrorFn> &_error)
  : SimpleObserver(std::nullopt, _error) 
{
  tkDebug() << "SimpleTypedObserver::SimpleTypedObserver()";
}

template <typename T>
inline SimpleTypedObserver<T>::SimpleTypedObserver(
  const std::optional<CompleteFn> &_complete)
  : SimpleObserver(std::nullopt, std::nullopt, _complete) 
{
  tkDebug() << "SimpleTypedObserver::SimpleTypedObserver()";
}

template <typename T>
inline SimpleTypedObserver<T>::SimpleTypedObserver(
    const std::optional<NextFn<T>> &_next, const std::optional<ErrorFn> &_error)
    : SimpleObserver(std::nullopt, _error) {
  m_next_T = _next;

  tkDebug() << "SimpleTypedObserver::SimpleTypedObserver()";
}

template <typename T>
inline SimpleTypedObserver<T>::SimpleTypedObserver(
    const std::optional<NextFn<T>> &_next, const std::optional<ErrorFn> &_error,
    const std::optional<CompleteFn> &_complete)
    : SimpleObserver(std::nullopt, _error, _complete) {
  m_next_T = _next;

  tkDebug() << "SimpleTypedObserver::SimpleTypedObserver()";
}

template <typename T> inline void SimpleTypedObserver<T>::next(const T &value) {
  tkDebug() << "SimpleTypedObserver::next()";

  if (m_next.has_value()) {
    m_next.value()(nullptr);
  }

  if (m_next_T.has_value()) {
    m_next_T.value()(value);
  }
}

#endif // SIMPLE_TYPED_OBSERVER_TPP