#ifndef STATUS_REGISTER_H
#define STATUS_REGISTER_H

#include <TkArch/Basic.h>

enum StatusMask {
  STATUS_MASK_CARRY     = 0b00000001,
  STATUS_MASK_ZERO      = 0b00000010,
  STATUS_MASK_NEGATIVE  = 0b00000100,
  STATUS_MASK_OVERFLOW  = 0b00001000,
  STATUS_MASK_BREAK     = 0b00010000,
  STATUS_MASK_UNUSED    = 0b00100000,
  STATUS_MASK_INTERRUPT = 0b01000000,
  STATUS_MASK_DECIMAL   = 0b10000000
};

class StatusRegister : public Register
{
public:
  StatusRegister(Clock *clock);
  virtual ~StatusRegister();
};

#endif