#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <TkArch/Basic.h>

class ControlUnit
{
protected:
  Clock *_clock;

public:
  ControlUnit(Clock *clock);
  ~ControlUnit();

public:
};

#endif
