#ifndef CAN_TEARDOWN_H
#define CAN_TEARDOWN_H

#include <functional>

#include <TkArch/Meta.h>

class Subscription;
class CanUnsubscribe;

class CanTeardown
{
  OBJECT(CanTeardown)

protected:
  std::function<void(void)> _teardown;

public:
  CanTeardown() = default;
  CanTeardown(const std::function<void(void)> &teardown);
  virtual ~CanTeardown() = default;

  virtual void teardown();
  virtual const std::function<void(void)> *reference();

public:
  static std::function<void(void)> toFunction(CanTeardown *ct);
  static CanTeardown *fromFunction(const std::function<void(void)> &f);
};

#ifndef cleanup
#define cleanup(body) CanTeardown::fromFunction(body)
#endif

#include "CanTeardown.tpp"

#endif