#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <functional>
#include <vector>

#include "CanTeardown.h"
#include "LikeSubscription.h"

#include <TkArch/Meta.h>

class CanUnsubscribe;

class Subscription : public LikeSubscription, public virtual CanTeardown
{
  OBJECT(Subscription)

protected:
  std::vector<Subscription *> _parents;
  std::vector<CanTeardown *> _teardowns;

  std::function<void(void)> _teardown;
  bool _closed = false;

public:
  Subscription(CanTeardown *teardown = nullptr);
  virtual ~Subscription();

  bool closed() const override;
  void unsubscribe() override;

  void teardown() override;

  void virtual add(Subscription *subscription);
  void add(CanUnsubscribe *unsubscribe);
  void add(const std::function<void(void)> teardown);

  void remove(Subscription *subscription);
  void remove(CanUnsubscribe *unsubscribe);
  void remove(const std::function<void(void)> teardown);

protected:
  bool _hasParent(Subscription *parent);
  void _addParent(Subscription *parent);
  void _removeParent(Subscription *parent);
};

static Subscription *emptySubscription();

inline Subscription *emptySubscription() {
  class EmptySubscription : public Subscription
  {
    OBJECT(EmptySubscription)

  protected:
    bool _closed = true;
    bool closed() const override { return _closed; }
  };

  return new EmptySubscription();
}

#endif