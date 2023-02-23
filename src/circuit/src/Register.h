#ifndef REGISTER_H
#define REGISTER_H

#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Clocked.h"
#include "Connected.h"

// This class has a weird name because it conflicts with the register keyword
class reg : public virtual connected, public virtual clocked
{
public:
  sp<service<isa::logic_t>> in_reset{0};
  sp<service<isa::logic_t>> in_load{0};
  sp<service<isa::data_t>> in_data{0};

  sp<service<isa::data_t>> out_data{0};

public:
  reg() {
    in_reset = std::make_shared<service<isa::logic_t>>(0);
    in_load = std::make_shared<service<isa::logic_t>>(0);
    in_data = std::make_shared<service<isa::data_t>>(0);

    out_data = std::make_shared<service<isa::data_t>>(0);

    watch<isa::logic_t>(in_clock, [this](isa::logic_t clk) {
      if (clk) {
        if (in_reset->value()) {
          out_data->next(0);
        } else if (in_load->value()) {
          out_data->next(in_data->value());
        }
      }
    });

    // report_if(in_reset, "Register - Register reset", &connected::truthy);
    // report_if(in_load, "Register - Register loaded", &connected::truthy);
    // report_if(in_data, "Register - Register received", &connected::truthy);
    // report(out_data, "Register - Register value");
  }

  virtual ~reg() {}
};

#endif