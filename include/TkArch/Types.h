#ifndef TYPES_H
#define TYPES_H

#include <functional>
#include <memory>

namespace types {

  // Functions
  template <typename in, typename out>
  using mapping = std::function<out(in)>;

  template <typename in>
  using mono_mapping = mapping<in, in>;

  template <typename in>
  using consumer = std::function<void(in)>;

  template <typename out>
  using producer = std::function<out(void)>;

  using side_effect = std::function<void(void)>;

  template <typename in>
  using next_t = consumer<in>;

  using error_t = consumer<std::exception>;

  using complete_t = side_effect;

  template <typename in>
  using predicate = mapping<in, bool>;

  template <typename obj>
  using sp = std::shared_ptr<obj>;

  template <typename obj>
  using wp = std::weak_ptr<obj>;

} // namespace types

template <typename T>
using sp = std::shared_ptr<T>;

template <typename T>
using wp = std::weak_ptr<T>;

template <typename T, typename... args>
static inline sp<T> new_sp(args &&...arguments) {
  return std::make_shared<T>(std::forward<args>(arguments)...);
}

#endif