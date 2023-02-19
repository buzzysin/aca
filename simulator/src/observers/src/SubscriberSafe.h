#ifndef SUBSCRIBER_SAFE_H
#define SUBSCRIBER_SAFE_H

#include <functional>
#include <optional>

#include <TkArch/Meta.h>

#include "Subscriber.h"

template <typename T>
class SubscriberSafe : public Subscriber<T, T>
{
  OBJECT(SubscriberSafe)

public:
  SubscriberSafe(SimpleTypedObserver<T> *destination);

  SubscriberSafe(
      std::optional<std::function<void(T &)>> next,
      std::optional<std::function<void(const std::exception &)>> error,
      std::optional<std::function<void()>> complete);

  virtual ~SubscriberSafe() = default;
};

#include "SubscriberSafe.tpp"

#endif // SUBSCRIBER_SAFE_H