#ifndef ADDRESSABLE_REGISTER_H
#define ADDRESSABLE_REGISTER_H

#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Register.h"

class addressable_register : public register_
{
protected:
  isa::addr_t _addr{0};

public:
  addressable_register(isa::addr_t addr) : _addr(addr) {}

  virtual ~addressable_register() {
    // std::cout << "Destroying addressable_register" << std::endl;
  }
};

#endif