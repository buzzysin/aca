#ifndef OF_TPP
#define OF_TPP

#include "from.h"
#include "of.h"

template <typename T = void *> auto of() -> Observable<T> * {
  return from(std::vector<T>({}));
}

template <typename T, typename... Rest>
auto of(const T &value, const Rest &...rest) -> Observable<T> * {
  return from(std::vector<T>({value, rest...}));
}

#endif
