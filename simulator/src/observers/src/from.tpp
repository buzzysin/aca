#ifndef FROM_TPP
#define FROM_TPP

#include "from.h"

template <typename T, template <class, class...> class Container, class... A>
auto from() -> Observable<T> * {
  return Observable<T>::empty();
}

template <typename T, template <class, class...> class Container, class... A>
auto from(const Container<T, A...> &value) -> Observable<T> * {
  if (value.empty()) {
    return emptyObservable<T>();
  }

  return new Observable<T>([=](Subscriber<T> *subscriber) {
    for (auto item : value) {
      subscriber->next(item);
    }

    subscriber->complete();

    return cleanup([]() {});
  });
}

#endif