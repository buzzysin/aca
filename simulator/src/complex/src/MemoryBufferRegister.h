#ifndef MEMORY_BUFFER_REGISTER_H
#define MEMORY_BUFFER_REGISTER_H

#include <TkArch/Basic.h>

class MemoryBufferRegister : public Register
{

public:
  MemoryBufferRegister( Clock *clock);
  virtual ~MemoryBufferRegister();
};

#endif