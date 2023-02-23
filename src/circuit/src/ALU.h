#ifndef ALU_H
#define ALU_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Clocked.h"
#include "Connected.h"

class alogic_unit : public virtual clocked, public virtual connected
{

public:
  sp<service<isa::data_t>> in_a;
  sp<service<isa::data_t>> in_b;

  sp<service<isa::logic_t>> in_mode;
  sp<service<isa::logic_t>> in_oe_data;

  sp<service<isa::logic_t>> out_z;
  sp<service<isa::logic_t>> out_c;
  sp<service<isa::logic_t>> out_o;
  sp<service<isa::logic_t>> out_n;

  sp<service<isa::data_t>> out_data;

  alogic_unit() {
    in_a = std::make_shared<service<isa::data_t>>(0);
    in_b = std::make_shared<service<isa::data_t>>(0);

    in_mode    = std::make_shared<service<isa::logic_t>>(0);
    in_oe_data = std::make_shared<service<isa::logic_t>>(0);

    out_z = std::make_shared<service<isa::logic_t>>(0);
    out_c = std::make_shared<service<isa::logic_t>>(0);
    out_o = std::make_shared<service<isa::logic_t>>(0);
    out_n = std::make_shared<service<isa::logic_t>>(0);

    out_data = std::make_shared<service<isa::data_t>>(0);

    watch<isa::data_t>(this->in_a,
                       [this](isa::data_t value) { IGNORE(value); });

    watch<isa::data_t>(this->in_b,
                       [this](isa::data_t value) { IGNORE(value); });

    watch<isa::logic_t>(this->in_mode,
                        [this](isa::logic_t value) { IGNORE(value); });

    report_if(in_a, "ALU", "A received", &connected::truthy);
    report_if(in_b, "ALU", "B received", &connected::truthy);
    report_if(in_mode, "ALU", "Mode received", &connected::truthy);
    report_if(in_oe_data, "ALU", "Output enable received", &connected::truthy);

    report(out_z, "ALU", "Zero flag");
    report(out_c, "ALU", "Carry flag");
    report(out_o, "ALU", "Overflow flag");
    report(out_n, "ALU", "Negative flag");

    report(out_data, "ALU", "Result");
  }
};

#endif