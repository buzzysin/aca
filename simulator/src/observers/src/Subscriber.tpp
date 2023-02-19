#ifndef SUBSCRIBER_TPP
#define SUBSCRIBER_TPP

#include "Subscriber.h"

template <typename T, typename U>
inline Subscriber<T, U>::Subscriber() : Subscription(), Observer<T>() {
  tkDebug() << "Subscriber::Subscriber()";

  _destination = emptyObserver<U>();
}

template <typename T, typename U>
inline Subscriber<T, U>::Subscriber(Subscriber<U> *destination)
    : Subscription(), Observer<T>() {
  tkDebug() << "Subscriber::Subscriber()";

  destination->add(this);

  _destination = destination;
}

template <typename T, typename U>
inline Subscriber<T, U>::Subscriber(Observer<U> *destination)
    : Subscription(), Observer<T>() {
  tkDebug() << "Subscriber::Subscriber()";

  _destination = destination;
}

template <typename T, typename U>
inline Subscriber<T, U>::~Subscriber() {
  tkDebug() << "Subscriber::~Subscriber()";

  if (_destination != nullptr) {
    delete _destination;
  }
}

template <typename T, typename U>
inline void Subscriber<T, U>::next(const T &value) {
  tkDebug() << "Subscriber::next()";

  if (_isStopped) {
    return;
  }

  this->_next(value);
}

template <typename T, typename U>
inline void Subscriber<T, U>::error( std::exception &error) {
  tkDebug() << "Subscriber::error()";

  if (_isStopped) {
    return;
  }

  _isStopped = true;

  this->_error(error);
}

template <typename T, typename U>
inline void Subscriber<T, U>::complete() {
  tkDebug() << "Subscriber::complete()";

  if (_isStopped) {
    return;
  }

  _isStopped = true;

  this->_complete();
}

template <typename T, typename U>
inline void Subscriber<T, U>::_next(const T &value) {
  tkDebug() << "Subscriber::_next()";

  _destination->next(value);
}

template <typename T, typename U>
inline void Subscriber<T, U>::_error( std::exception &error) {
  tkDebug() << "Subscriber::_error()";

  try {
    _destination->error(error);
  } catch ( std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
  }

  this->unsubscribe();
}

template <typename T, typename U>
inline void Subscriber<T, U>::_complete() {
  tkDebug() << "Subscriber::_complete()";

  try {
    _destination->complete();
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << "\n";
  }

  this->unsubscribe();
}

#endif
