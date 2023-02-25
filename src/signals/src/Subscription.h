#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <iostream>
#include <memory>
#include <vector>

#include <TkArch/Types.h>

#include "Types.h"



class subscription : public virtual subscription_like,
                     public std::enable_shared_from_this<subscription>
{
protected:
  bool _is_closed = false;
  std::vector<wp<subscription>> _others;

  types::side_effect _side_effect;
  std::vector<types::side_effect> _side_effects;

public:
  subscription() {}
  subscription(const types::side_effect &side_effect) {
    _side_effect = side_effect;
  }

  virtual ~subscription() {
    // std::cerr << "subscription::~subscription()" << std::endl;
  }

  virtual void unsubscribe() {
    if (_is_closed)
      return;

    std::vector<std::exception> errors;

    _is_closed = true;

    for (auto &other : _others) {
      try {
        auto _other = other.lock();

        if (_other)
          _other->unsubscribe();
      } catch (std::exception &e) {
        errors.push_back(e);
      }
    }

    if (_side_effect) {
      try {
        _side_effect();
      } catch (std::exception &e) {
        errors.push_back(e);
      }
    }

    for (auto &side_effect : _side_effects) {
      try {
        side_effect();
      } catch (std::exception &e) {
        errors.push_back(e);
      }
    }

    if (errors.size() > 0) {
      throw std::runtime_error("Errors occured while closing connection");
    }
  }

  virtual void add(wp<subscription> other) {
    auto _other = other.lock();

    if (!_other)
      return;

    if (_is_closed) {
      _other->unsubscribe();
      return;
    }

    if (_other->is_closed()) {
      return;
    }

    if (_other == shared_from_this())
      return;

    _others.push_back(other);
  }

  virtual void add(sp<subscription> other) {
    if (!other)
      return;

    if (_is_closed) {
      other->unsubscribe();
      return;
    }

    if (other->is_closed()) {
      return;
    }

    if (other == shared_from_this())
      return;

    _others.push_back(wp<subscription>(other));
  }

  virtual void add(const types::side_effect &side_effect) {
    if (!side_effect)
      return;

    if (_is_closed) {
      try {
        side_effect();
      } catch (std::exception &e) {
        // ignore
      }

      return;
    }

    _side_effects.push_back(side_effect);
  }

  virtual void add(const std::monostate &) {
    // ignore
  }

  virtual void remove(wp<subscription> other) {
    auto _other = other.lock();

    auto it =
        std::find_if(_others.begin(), _others.end(), [&](wp<subscription> o) {
          return o.lock() == _other;
        });

    if (it != _others.end()) {
      _others.erase(it);
    }
  }

  virtual void remove(sp<subscription> other) {

    auto it =
        std::find_if(_others.begin(), _others.end(), [&](wp<subscription> o) {
          return o.lock() == other;
        });

    if (it != _others.end()) {
      _others.erase(it);
    }
  }

  virtual void remove(const types::side_effect &side_effect) {
    auto it = std::find_if(_side_effects.begin(),
                           _side_effects.end(),
                           [&](auto o) { return &o == &side_effect; });

    if (it != _side_effects.end()) {
      _side_effects.erase(it);
    }
  }

  virtual void remove(const std::monostate &) {
    // nothing to do
  }

  virtual bool is_closed() const override { return _is_closed; }
};

class empty_subscription : public subscription
{
protected:
  bool _is_closed = true;
};

#endif // SUBSCRIPTION_H