#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <variant>

#include "Observer.h"
#include "Subscriber.h"
#include "Subscription.h"
#include "Types.h"

template <typename T>
class observable : public virtual subscribable<T>
{
protected:
  struct ss : public sp<subscription> {
    using sp<subscription>::sp;
    ss(void) = delete; // stop void* from being converted to sp
  };
  using subscribe_sb = types::mapping<sp<subscriber<T>>, ss>;
  using subscribe_fn = types::mapping<sp<subscriber<T>>, types::side_effect>;
  using subscribe_na = types::consumer<sp<subscriber<T>>>;

  subscribe_sb _inner_subscribe_sb;
  subscribe_fn _inner_subscribe_fn;
  subscribe_na _inner_subscribe_na;

public:
  observable() {}

  observable(const subscribe_sb &subscribe) { _inner_subscribe_sb = subscribe; }
  observable(const subscribe_fn &subscribe) { _inner_subscribe_fn = subscribe; }
  observable(const subscribe_na &subscribe) { _inner_subscribe_na = subscribe; }

  virtual ~observable() {
    // std::cerr << "observable::~observable()" << std::endl;
  }

  virtual sp<subscription> subscribe(sp<observer<T>> obs) {
    sp<subscription> conn;

    auto _sub = new_sp<subscriber<T>>(new_sp<subscriber_no_throw<T>>(obs));

    std::variant<sp<subscription>, types::side_effect, std::monostate> result;

    if (_inner_subscribe_sb) {
      _sub->add(_inner_subscribe_sb(_sub));
    } else if (_inner_subscribe_fn) {
      _sub->add(_inner_subscribe_fn(_sub));
    } else if (_inner_subscribe_na) {
      _inner_subscribe_na(_sub);
      _sub->add(std::monostate());
    }

    return _sub;
  }

  sp<subscription> subscribe(const types::next_t<T> &next) {
    return subscribe(new_sp<observer_no_throw<T>>(next));
  }
};

#endif // OBSERVABLE_H