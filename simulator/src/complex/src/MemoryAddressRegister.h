#ifndef MEMORY_ADDRESS_REGISTER_H
#define MEMORY_ADDRESS_REGISTER_H

#include <TkArch/Basic.h>

class MemoryAddressRegister : public Register
{

public:
  MemoryAddressRegister(Clock *clock);
  virtual ~MemoryAddressRegister();
};

#endif