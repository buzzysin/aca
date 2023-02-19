#ifndef OBSERVER_CONSUMER_TPP
#define OBSERVER_CONSUMER_TPP

#include "ObserverConsumer.h"

template <typename T>
inline ObserverConsumer<T>::ObserverConsumer(
    SimpleTypedObserver<T> *partialObserver)
    : Observer<T>() {
  tkDebug() << "ObserverConsumer::ObserverConsumer()";

  _partialObserver = partialObserver;
}

template <typename T> inline void ObserverConsumer<T>::next(const T &value) {
  tkDebug().nolines() << "ObserverConsumer::next() with value = " << value
                      << "\n";

  try {
    _partialObserver->next(value);
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << "\n";
  }
}

template <typename T>
inline void ObserverConsumer<T>::error( std::exception &error) {
  tkDebug() << "ObserverConsumer::error()";

  try {
    _partialObserver->error(error);
  } catch ( std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

template <typename T> inline void ObserverConsumer<T>::complete() {
  tkDebug() << "ObserverConsumer::complete()";

  try {
    _partialObserver->complete();
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << "\n";
  }
}

#endif