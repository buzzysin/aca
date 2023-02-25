#ifndef SUBSCRIPTION_MANAGER_H
#define SUBSCRIPTION_MANAGER_H

#include <TkArch/Types.h>

#include "Subscription.h"

#include <vector>

class subscription_manager
{
  std::vector<sp<subscription>> _subscriptions;

public:
  virtual ~subscription_manager() {}

  void watch(sp<subscription> sub) { _subscriptions.push_back(sub); }

  template <template <class> typename Source, typename Mapping, typename A>
  void watch(const sp<Source<A>> &in, const Mapping &callback) {
    watch(in->subscribe(callback));
  }

  template <template <class> typename Source,
            template <class>
            typename Sink,
            typename Mapping,
            typename T,
            typename U>
  void watch(const sp<Source<T>> &out,
             const sp<Sink<U>> &in,
             const Mapping &callback) {
    watch(out->subscribe([&](const T &value) { in->next(callback(value)); }));
  }

  template <template <class> typename Source,
            template <class>
            typename Sink,
            typename T>
  void watch(const sp<Source<T>> &out, const sp<Sink<T>> &in) {
    watch(out, in, [](const T &value) { return value; });
  }

  template <template <class> typename Source,
            template <class>
            typename Sink,
            template <class>
            typename Buffer,
            typename T,
            typename U,
            typename V>
  void watch(const sp<Source<T>> &out,
             const sp<Sink<U>> &in,
             const sp<Buffer<V>> &enable) {

    watch(out->subscribe([&](const T &value) {
      if (enable->value()) {
        in->next(value);
      }
    }));

    watch(enable->subscribe([&](const V &value) {
      if (value) {
        in->next(out->value());
      }
    }));
  }
};

#endif