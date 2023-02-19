#ifndef OPERATOR_TPP
#define OPERATOR_TPP

#include "Operator.h"

template <typename T, typename U>
Operator<T, U>::Operator(
    const std::function<CanTeardown *(Subscriber<U> *, LiftedObservable<T> *)>
        &init)
    : _init(init) {}

template <typename T, typename U>
CanTeardown *Operator<T, U>::call(Subscriber<U> *subscriber,
                                  LiftedObservable<T> *source) {
  return _init(subscriber, source);
};

template <typename T, typename U>
CanTeardown *Operator<T, U>::operator()(Subscriber<U> *subscriber,
                                        LiftedObservable<T> *source) {
  return _init(subscriber, source);
}

template <typename T, typename U>
inline Operator<T, U>::operator std::function<
    CanTeardown *(Subscriber<U> *, LiftedObservable<T> *)>() {
  return _init;
}

#endif