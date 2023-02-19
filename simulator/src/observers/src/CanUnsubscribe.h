#ifndef CAN_UNSUBSCRIBE_H
#define CAN_UNSUBSCRIBE_H

#include "CanTeardown.h"

class CanUnsubscribe : public virtual CanTeardown
{
  OBJECT(CanUnsubscribe)

public:
  virtual ~CanUnsubscribe() = default;

  virtual void unsubscribe() = 0;
};

#endif // CAN_UNSUBSCRIBE_H