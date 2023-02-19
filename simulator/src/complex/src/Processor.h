#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <TkArch/Basic.h>

#include "ArithmeticLogicUnit.h"
#include "ControlUnit.h"
#include "MemoryAddressRegister.h"
#include "MemoryBufferRegister.h"

class Processor
{
protected:
  Clock *_clock;
  ControlUnit *_controlUnit;
  MemoryAddressRegister *_memoryAddressRegister;
  MemoryBufferRegister *_memoryBufferRegister;
  ArithmeticLogicUnit *_arithmeticLogicUnit;

public:
  Processor();
  virtual ~Processor();

public:
  void start();
  void stop();
};

#endif