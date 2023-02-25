#ifndef SIGNALS_TYPES_H
#define SIGNALS_TYPES_H

#include <functional>
#include <memory>

template <typename T>
class observer;

template <typename T>
class subscriber;

template <typename T>
class subject;

class subscription;

struct unsubscribable {
  virtual ~unsubscribable() {
    // std::cerr << "unsubscribable::~unsubscribable()" << std::endl;
  }
  virtual void unsubscribe() = 0;
};

struct subscription_like : public virtual unsubscribable {
  virtual ~subscription_like() {
    // std::cerr << "subscription_like::~subscription_like()" << std::endl;
  }
  virtual bool is_closed() const = 0;
  virtual void unsubscribe()     = 0;
};

template <typename T>
struct subscribable {
  virtual ~subscribable() {
    // std::cerr << "subscribable::~subscribable()" << std::endl;
  }
  virtual sp<subscription> subscribe(sp<observer<T>>) = 0;
};

template <typename T>
class observable;

template <typename T>
class observer_no_throw;

template <typename T>
class subject;

template <typename T>
class behaviour_subject;

template <typename T>
using emitter = subject<T>;
#endif

template <typename T>
using signal = behaviour_subject<T>;