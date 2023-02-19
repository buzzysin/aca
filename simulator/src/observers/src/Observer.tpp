#ifndef OBSERVER_TPP
#define OBSERVER_TPP

#include "Observer.h"

#include <TkArch/Meta.h>

template <typename T>
inline Observer<T>::Observer() : SimpleTypedObserver<T>() {
  tkDebug() << "Observer::Observer()";
}

template <typename T>
inline Observer<T>::Observer(const NextFn<T> &next)
    : SimpleTypedObserver<T>(next) {
  tkDebug() << "Observer::Observer()";
}

template <typename T>
inline Observer<T>::Observer(const ErrorFn &error)
    : SimpleTypedObserver<T>(error) {
  tkDebug() << "Observer::Observer()";
}

template <typename T>
inline Observer<T>::Observer(const CompleteFn &complete)
    : SimpleTypedObserver<T>(complete) {
  tkDebug() << "Observer::Observer()";
}

template <typename T>
inline Observer<T>::Observer(const NextFn<T> &next, const ErrorFn &error)
    : SimpleTypedObserver<T>(next, error) {
  tkDebug() << "Observer::Observer()";
}

template <typename T>
inline Observer<T>::Observer(const NextFn<T> &next, const ErrorFn &error,
                             const CompleteFn &complete)
    : SimpleTypedObserver<T>(next, error, complete) {
  tkDebug() << "Observer::Observer()";
}

template <typename T>
inline Observer<T>::~Observer() {
  tkDebug() << "Observer::~Observer()";
}

template <typename T>
inline Observer<T> *emptyObserver() {
  class EmptyObserver : public Observer<T>
  {
  public:
    void next(const T &) override { tkDebug() << "EmptyObserver::next()"; }
  };

  return new EmptyObserver();
};

#endif