#ifndef LIKE_SUBSCRIPTION_H
#define LIKE_SUBSCRIPTION_H

#include <TkArch/Meta.h>

#include "CanUnsubscribe.h"

class LikeSubscription : public CanUnsubscribe
{
  OBJECT(LikeSubscription)

public:
  virtual ~LikeSubscription() = default;

  virtual void unsubscribe() = 0;

  virtual bool closed() const = 0;
};

#endif // LIKE_SUBSCRIPTION_H