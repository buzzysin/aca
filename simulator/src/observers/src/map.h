#ifndef MAP_H
#define MAP_H

#include "OperatorSubscriber.h"
#include "operate.h"

template <typename T, typename U>
UnaryFunction<LiftedObservable<T> *, LiftedObservable<U> *>
map(const UnaryFunction<T, U> &f) {
  return operate<T, U>([=](LiftedObservable<T> *source, Subscriber<U> *lifted) {
    return createOperatorSubscriber<T, U>(source, lifted, [=](T t) {
      lifted->next(f(t));
    });
  });
}

#endif