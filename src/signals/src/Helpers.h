#ifndef HELPERS_H
#define HELPERS_H

#include <algorithm>

namespace helpers {

  template <template <class, class...> typename Container,
            typename T,
            typename... A>
  inline void vecRemove(Container<T, A...> &vec,  T &item) {
    vec.erase(std::remove(vec.begin(), vec.end(), item), vec.end());
  }

  template <template <class, class...> typename Container,
            typename T,
            typename... A>
  inline void vecRemoveWp(Container<T, A...> &vec, const T &item) {
    vec.erase(std::remove_if(
                  vec.begin(),
                  vec.end(),
                  [&item](const T &wp) { return wp.lock() == item.lock(); }),
              vec.end());
  }

  template <template <class, class...> typename Container,
            typename T,
            typename... A>
  inline bool vecHas(Container<T, A...> &vec, const T &item) {
    return std::find(vec.begin(), vec.end(), item) != vec.end();
  }

  template <template <class, class...> typename Container,
            typename T,
            typename... A>
  inline bool vecHasWp(Container<T, A...> &vec, const T &item) {
    return std::find_if(vec.begin(), vec.end(), [&item](const T &wp) {
             return wp.lock() == item.lock();
           }) != vec.end();
  }

} // namespace helpers

#endif