#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <variant>

#include "Observer.h"
#include "Subscriber.h"
#include "Subscription.h"

template <typename T>
class subscriber : public subscription, public observer<T>
{
  using observer<T>::_inner_next;
  using observer<T>::_inner_error;
  using observer<T>::_inner_complete;

  using subscription::_is_closed;

protected:
  bool _is_stopped = false;
  sp<subscriber<T>> _destination_subscriber;
  sp<observer<T>> _destination_observer;

public:
  subscriber() { _destination_observer = new_sp<observer_no_throw<T>>(); }

  subscriber(sp<observer<T>> obs) : subscriber() {
    auto _subscriber = std::dynamic_pointer_cast<subscriber<T>>(obs);

    if (_subscriber) {
      _destination_subscriber = _subscriber;
      _destination_observer   = nullptr;
    } else {
      _destination_observer   = obs;
      _destination_subscriber = nullptr;
    }
  }

  virtual ~subscriber() {
    // std::cerr << "subscriber::~subscriber()" << std::endl;
  }

  virtual void next(T value) override {

    if (_is_stopped)
      return;

    _next(value);
  }

  virtual void error(std::exception &value) override {

    if (_is_stopped)
      return;

    _is_stopped = true;
    _error(value);
  }

  virtual void complete() override {

    if (_is_stopped)
      return;

    _is_stopped = true;
    _complete();
  }

  virtual void _next(T value) {
    auto _observer   = _destination_observer;
    auto _subscriber = _destination_subscriber;

    if (_observer) {
      _observer->next(value);
    } else if (_subscriber) {
      _subscriber->next(value);
    } else {
      throw std::runtime_error("Internal error - no destination");
    }
  }

  virtual void _error(std::exception &value) {
    auto _observer   = _destination_observer;
    auto _subscriber = _destination_subscriber;

    try {
      if (_observer) {
        _observer->error(value);
      } else if (_subscriber) {
        _subscriber->error(value);
      } else {
        throw std::runtime_error("Internal error - no destination");
      }
    } catch (std::exception &e) {
      // ignore
    }

    subscription::unsubscribe();
  }

  virtual void _complete() {
    auto _observer   = _destination_observer;
    auto _subscriber = _destination_subscriber;

    try {
      if (_observer) {
        _observer->complete();
      } else if (_subscriber) {
        _subscriber->complete();
      } else {
        throw std::runtime_error("Internal error - no destination");
      }
    } catch (std::exception &e) {
      // ignore
    }

    subscription::unsubscribe();
  }
};

template <typename T>
class subscriber_no_throw : public subscriber<T>
{
protected:
  using subscriber<T>::_is_stopped;
  using subscriber<T>::_destination_subscriber;
  using subscriber<T>::_destination_observer;

public:
  using subscriber<T>::subscriber;

  virtual ~subscriber_no_throw() {
    std::cerr << "subscriber_no_throw::~subscriber_no_throw()" << std::endl;
  }

  virtual void _next(T value) override {
    try {
      subscriber<T>::_next(value);
    } catch (std::exception &e) {
      std::cerr << "An exception occured: " << e.what() << std::endl;
    }
  }

  virtual void _error(std::exception &value) override {
    try {
      subscriber<T>::_error(value);
    } catch (std::exception &e) {
      std::cerr << "An exception occured: " << e.what() << std::endl;
    }
  }

  virtual void _complete() override {
    try {
      subscriber<T>::_complete();
    } catch (std::exception &e) {
      std::cerr << "An exception occured: " << e.what() << std::endl;
    }
  }
};

#endif // SUBSCRIBER_H