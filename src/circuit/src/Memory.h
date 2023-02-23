#ifndef MEMORY_H
#define MEMORY_H

#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Clocked.h"
#include "Connected.h"
#include "Register.h"

class memory : public clocked, public virtual connected
{
protected:
  std::vector<reg> _memory;

public:
  sp<service<isa::logic_t>> in_write;
  sp<service<isa::logic_t>> in_read;

  sp<service<isa::logic_t>> out_write;
  sp<service<isa::logic_t>> out_read;

  sp<service<isa::data_t>> addr_bus;
  sp<service<isa::data_t>> data_bus;

public:
  memory(size_t size) {
    in_write = std::make_shared<service<isa::logic_t>>(0);
    in_read  = std::make_shared<service<isa::logic_t>>(0);

    out_write = std::make_shared<service<isa::logic_t>>(0);
    out_read  = std::make_shared<service<isa::logic_t>>(0);

    addr_bus = std::make_shared<service<isa::data_t>>(0);
    data_bus = std::make_shared<service<isa::data_t>>(0);

    _memory.resize(size);

    std::for_each(_memory.begin(), _memory.end(), [&](reg &r) {
      redirect(in_clock, r.in_clock);
    });

    watch<isa::logic_t>(in_clock, [this](isa::logic_t clk) {
      reg registr = _memory[addr_bus->value()];
      if (clk) {
        if (in_write->value()) {
          registr.in_data->next(data_bus->value());
        } else if (in_read->value()) {
          data_bus->next(registr.out_data->value());
        }
      }
    });

    report_if(in_write, "Memory", "Write", &connected::truthy);
    report_if(in_read, "Memory", "Read", &connected::truthy);
    report(addr_bus, "Memory", "Address");
    report(data_bus, "Memory", "Data");
  }
};

#endif