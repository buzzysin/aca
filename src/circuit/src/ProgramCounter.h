#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>
#include <TkArch/Types.h>

#include "Counter.h"

class program_counter : public subscription_manager
{
public:
  counter count;

  sp<signal<isa::data_t>> out_data;

public:
  program_counter() {
    out_data = std::make_shared<signal<isa::data_t>>(0);

    watch(count.out_data, out_data);

    watch(count.in_data, [this](isa::data_t data) {
      create_printer("Program Counter");
      print_info("Program Counter received: " + std::to_string((int)data));
    });

    watch(count.out_data, [this](isa::data_t data) {
      create_printer("Program Counter");
      print_info("Program Counter sending: " + std::to_string((int)data));
    });
  }

  virtual ~program_counter() {}
};

#endif
