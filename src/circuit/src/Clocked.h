#ifndef CLOCKED_H
#define CLOCKED_H

#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

class clocked
{
public:
  sp<service<isa::logic_t>> in_clock{0};

public:
  clocked() { in_clock = std::make_shared<service<isa::logic_t>>(0); }
  virtual ~clocked() {}
};

#endif