#ifndef OPERATOR_H
#define OPERATOR_H

#include <functional>

#include "Types.h"

#include "CanTeardown.h"
#include "LiftedObservable.h"
#include "Subscriber.h"

template <typename U, typename T = U>
class Operator
{
protected:
  std::function<CanTeardown *(Subscriber<T> *, LiftedObservable<U> *)> _init;

public:
  Operator(const std::function<CanTeardown *(Subscriber<T> *,
                                             LiftedObservable<U> *)> &init);
  virtual ~Operator() = default;

  CanTeardown *call(Subscriber<T> *subscriber, LiftedObservable<U> *source);

  CanTeardown *operator()(Subscriber<T> *subscriber,
                          LiftedObservable<U> *source);

  operator std::function<CanTeardown *(Subscriber<T> *,
                                       LiftedObservable<U> *)>();
};

#include "Operator.tpp"

#endif