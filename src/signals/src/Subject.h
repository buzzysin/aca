#ifndef SUBJECT_H
#define SUBJECT_H

#include <iostream>
#include <memory>
#include <vector>

#include "Observable.h"
#include "Observer.h"
#include "Subscription.h"

template <typename T>
class subject : public observable<T>,
                public subscription_like,
                public virtual observer<T>
{
protected:
  std::vector<sp<observer<T>>> _observers;
  std::vector<sp<observer<T>>> _current_observers;

  bool _closed     = false;
  bool _is_stopped = false;
  bool _errored    = false;
  std::exception _error;

public:
  using observable<T>::subscribe;

public:
  subject() {}

  virtual ~subject() {
    // std::cerr << "subject::~subject()" << std::endl;
  }

  virtual void next(T value) {
    _throw_if_closed();

    if (_is_stopped)
      return;

    if (_current_observers.empty()) {
      _current_observers = _observers;
    }

    for (auto &obs : _current_observers) {
      obs->next(value);
    }
  }

  virtual void error(std::exception &value) {
    _throw_if_closed();

    if (_is_stopped)
      return;

    _is_stopped = true;
    _errored    = true;

    while (!_observers.empty()) {
      auto obs = _observers.back();

      obs->error(value);

      _observers.pop_back();
    }
  }

  virtual void complete() {
    _throw_if_closed();

    if (_is_stopped)
      return;

    _is_stopped = true;

    while (!_observers.empty()) {
      auto obs = _observers.back();

      obs->complete();

      _observers.pop_back();
    }
  }

  virtual sp<subscription> subscribe(sp<observer<T>> obs) override {
    _throw_if_closed();

    _check_if_stopped_or_errored(obs);

    return _inner_subscribe(obs);
  }

  virtual sp<subscription> _inner_subscribe(sp<observer<T>> obs) {

    if (_is_stopped || _errored) {
      return new_sp<empty_subscription>();
    }

    _current_observers.clear();
    _observers.push_back(obs);

    return new_sp<subscription>([this, obs] {
      _current_observers.clear();
      std::remove_if(_observers.begin(),
                     _observers.end(),
                     [obs](sp<observer<T>> &_obs) { return _obs == obs; });
    });
  }

  virtual void unsubscribe() override {
    _closed = _is_stopped = true;
    _observers.clear();
    _current_observers.clear();
  }

public:
  virtual bool is_closed() const override { return _closed; }

protected:
  void _throw_if_closed() const {
    if (_closed) {
      throw std::runtime_error("Subject has been closed");
    }
  }

  void _check_if_stopped_or_errored(sp<observer<T>> sub) {
    if (_errored) {
      sub->error(_error);
    } else if (_is_stopped) {
      sub->complete();
    }
  }
};

#endif // SUBJECT_H