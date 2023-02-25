#ifndef REGISTER_H
#define REGISTER_H

#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

class register_ : public subscription_manager
{
protected:
  isa::data_t _data{0}, _next_data{0};

public:
  sp<signal<isa::logic_t>> in_clock;
  sp<signal<isa::logic_t>> in_reset;
  sp<signal<isa::logic_t>> in_load;
  sp<signal<isa::data_t>> in_data;

  sp<signal<isa::data_t>> out_data;

public:
  register_() {
    in_clock = std::make_shared<signal<isa::logic_t>>(0);
    in_reset = std::make_shared<signal<isa::logic_t>>(0);
    in_load  = std::make_shared<signal<isa::logic_t>>(0);
    in_data  = std::make_shared<signal<isa::data_t>>(0);

    out_data = std::make_shared<signal<isa::data_t>>(0);

    setup();
  }

  void setup() {
    watch(in_clock, [this](isa::logic_t clk) {
      if (clk) {
        
        create_printer("Register");
        print_info("Current Data: " + std::to_string(_data));

        _data = next_data(_data);
        next_outputs(_data);

        print_info("New Data: " + std::to_string(_data));
      }
    });
  }

  isa::data_t next_data(isa::data_t data) {
    create_printer("Register");

    isa::data_t new_data = data;

    if (in_reset->value()) {
      print_debug("Resetting data");
      new_data = 0;
    } else if (in_load->value()) {
      print_debug("Loading data");
      new_data = in_data->value();
    } else {
      print_debug("No change");
    }

    return new_data;
  }

  void next_outputs(isa::data_t data) { out_data->next(data); }
};

#endif
