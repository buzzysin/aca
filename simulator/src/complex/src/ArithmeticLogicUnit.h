#ifndef ARITHMETIC_LOGIC_UNIT_H
#define ARITHMETIC_LOGIC_UNIT_H

#include <TkArch/Basic.h>

class ArithmeticLogicUnit
{
protected:
  const Clock *_clock;

public:
  ArithmeticLogicUnit(const Clock *clock);
  virtual ~ArithmeticLogicUnit();
};

#endif // ARITHMETIC_LOGIC_UNIT_H
