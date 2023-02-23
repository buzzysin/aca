#ifndef TYPES_H
#define TYPES_H

#include <exception>
#include <functional>
#include <memory>

namespace types {
  template <typename T>
  using consumer = std::function<void(T)>;

  template <typename T>
  using next_fn = consumer<T>;

  using error_fn = std::function<void(const std::exception &e)>;

  using complete_fn = std::function<void()>;

  using dc_fn = std::function<void()>;

  template  <typename T, typename U>
  using mapping = std::function<U(T)>;

  template <typename T>
  using mono_mapping = std::function<T(T)>;

  template <typename T>
  using predicate = std::function<bool(T)>;

  


  template <typename T>
  using wp = std::weak_ptr<T>;

  template <typename T>
  using sp = std::shared_ptr<T>;

} // namespace types

template <typename T>
using wp = types::wp<T>;

template <typename T>
using sp = types::sp<T>;

#endif