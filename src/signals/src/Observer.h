#ifndef OBSERVER_H
#define OBSERVER_H

#include <TkArch/Types.h>

#include <iostream>
#include <stdexcept>
#include <variant>

template <typename T>
class observer
{

protected:
  types::next_t<T> _inner_next;
  types::error_t _inner_error;
  types::complete_t _inner_complete;

public:
  observer() {}
  observer(const types::next_t<T> &next) { _inner_next = next; }
  observer(const types::error_t &error) { _inner_error = error; }
  observer(const types::complete_t &complete) { _inner_complete = complete; }
  observer(const types::next_t<T> &next, const types::error_t &error) {
    _inner_next  = next;
    _inner_error = error;
  }
  observer(const types::next_t<T> &next, const types::complete_t &complete) {
    _inner_next     = next;
    _inner_complete = complete;
  }
  observer(const types::error_t &error, const types::complete_t &complete) {
    _inner_error    = error;
    _inner_complete = complete;
  }
  observer(const types::next_t<T> &next,
           const types::error_t &error,
           const types::complete_t &complete) {
    _inner_next     = next;
    _inner_error    = error;
    _inner_complete = complete;
  }

  virtual ~observer() {
    //  std::cerr << "observer::~observer()" << std::endl;
  }

  virtual void next(T value) {
    if (_inner_next) {
      _inner_next(value);
    } else {
      throw std::runtime_error("No next handler");
    }
  }

  virtual void error(std::exception &value) {
    if (_inner_error) {
      _inner_error(value);
    } else {
      throw std::runtime_error("No error handler");
    }
  }

  virtual void complete() {
    if (_inner_complete) {
      _inner_complete();
    } else {
      throw std::runtime_error("No complete handler");
    }
  }
};

template <typename T>
class observer_no_throw : public observer<T>
{
  using observer<T>::_inner_next;
  using observer<T>::_inner_error;
  using observer<T>::_inner_complete;

public:
  observer_no_throw() {}
  observer_no_throw(const types::next_t<T> &next) : observer<T>(next) {}
  observer_no_throw(const types::error_t &error) : observer<T>(error) {}
  observer_no_throw(const types::complete_t &complete)
      : observer<T>(complete) {}
  observer_no_throw(const types::next_t<T> &next, const types::error_t &error)
      : observer<T>(next, error) {}
  observer_no_throw(const types::next_t<T> &next,
                    const types::complete_t &complete)
      : observer<T>(next, complete) {}
  observer_no_throw(const types::error_t &error,
                    const types::complete_t &complete)
      : observer<T>(error, complete) {}
  observer_no_throw(const types::next_t<T> &next,
                    const types::error_t &error,
                    const types::complete_t &complete)
      : observer<T>(next, error, complete) {}

  virtual ~observer_no_throw() {
    // std::cerr << "observer_no_throw::~observer_no_throw()" << std::endl;
  }

  virtual void next(T value) override {
    if (_inner_next) {
      try {
        _inner_next(value);
      } catch (std::exception &e) {
        // ignore
      }
    }
  }

  virtual void error(std::exception &value) override {
    if (_inner_error) {
      try {
        _inner_error(value);
      } catch (std::exception &e) {
        // ignore
      }
    }
  }

  virtual void complete() override {
    if (_inner_complete) {
      try {
        _inner_complete();
      } catch (std::exception &e) {
        // ignore
      }
    }
  }
};

#endif