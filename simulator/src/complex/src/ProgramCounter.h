#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#include <TkArch/Basic.h>

class ProgramCounter : public Register
{
public:
  ProgramCounter(Clock *clock);
  virtual ~ProgramCounter();
};

#endif
