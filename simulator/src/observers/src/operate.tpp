#ifndef OPERATE_TPP
#define OPERATE_TPP

#include "operate.h"

#include "Types.h"

#include "CanTeardown.h"
#include "LiftedObservable.h"
#include "Subscriber.h"

template <typename ObsT, typename SubU>
using InitFunction = std::function<CanTeardown *(const ObsT &, const SubU &)>;

template <typename T, typename U>
static UnaryFunction<LiftedObservable<T> *, LiftedObservable<U> *>
operate(const InitFunction<LiftedObservable<T> *, Subscriber<U> *> &init) {
  return [=](LiftedObservable<T> *source) {
    return source->lift([=](Subscriber<U> *lifted) {
      try {
        return init(source, lifted);
      } catch (std::exception &e) {
        lifted->error(e);
        return cleanup([]() {});
      }
    });
  };
}

#endif