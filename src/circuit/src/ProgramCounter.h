#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Counter.h"

class program_counter : public counter
{
protected:
  counter _counter;
  sp<service<isa::data_t>> _buffer{0};

public:
  sp<service<isa::logic_t>> in_oe_data{0};
  sp<service<isa::data_t>> out_data{0};

public:
  program_counter() {
    _buffer = std::make_shared<service<isa::data_t>>(0);

    in_oe_data = std::make_shared<service<isa::logic_t>>(0);
    out_data   = std::make_shared<service<isa::data_t>>(0);

    _counter.in_increment->next(1); // will never count down

    redirect(in_clock, _counter.in_clock);
    redirect(in_reset, _counter.in_reset);
    redirect(in_load, _counter.in_load);
    redirect(in_data, _counter.in_data);

    redirect(_storage.out_data, _buffer);
    watch_buffer(_buffer, out_data, in_oe_data);

    // report_if(in_clock, "PC", "Clocked", &connected::truthy);
    report_if(in_load, "PC", "Loaded", &connected::truthy);
    report_if(in_reset, "PC", "Reset", &connected::truthy);
    report_if(
        in_oe_data, "PC", "Enabled", &connected::truthy);
    report_if(in_data, "PC", "Received", &connected::truthy);
  }

  virtual ~program_counter() {}
};

#endif