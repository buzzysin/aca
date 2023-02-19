#ifndef SUBSCRIBER_SAFE_TPP
#define SUBSCRIBER_SAFE_TPP

#include "SubscriberSafe.h"

#include "ObserverConsumer.h"

template <typename T>
inline SubscriberSafe<T>::SubscriberSafe(SimpleTypedObserver<T> *destination)
    : Subscriber<T>() {
  tkDebug() << "SubscriberSafe::SubscriberSafe()";

  this->_destination = new ObserverConsumer<T>(destination);
}

template <typename T>
inline SubscriberSafe<T>::SubscriberSafe(
    std::optional<std::function<void(T &)>> next,
    std::optional<std::function<void(const std::exception &)>> error,
    std::optional<std::function<void()>> complete)
    : Subscriber<T>(next, error, complete) {
  tkDebug() << "SubscriberSafe::SubscriberSafe()";

  auto partialObserver = new SimpleObserver(next, error, complete);

  this->_destination = new ObserverConsumer<T>(partialObserver);
}

#endif // SUBSCRIBER_SAFE_TPP