#ifndef COUNTER_H
#define COUNTER_H

#include <TkArch/Signals.h>

class counter : public subscription_manager
{
protected:
  isa::data_t _count{0}, _next_count{0};

public:
  sp<signal<isa::logic_t>> in_clock;
  sp<signal<isa::logic_t>> in_reset;
  sp<signal<isa::logic_t>> in_load;
  sp<signal<isa::logic_t>> in_inc;
  sp<signal<isa::data_t>> in_data;

  sp<signal<isa::data_t>> out_data;

public:
  counter() {
    in_clock = std::make_shared<signal<isa::logic_t>>(0);
    in_reset = std::make_shared<signal<isa::logic_t>>(0);
    in_load  = std::make_shared<signal<isa::logic_t>>(0);
    in_inc   = std::make_shared<signal<isa::logic_t>>(1);
    in_data  = std::make_shared<signal<isa::data_t>>(0);

    out_data = std::make_shared<signal<isa::data_t>>(0);

    setup();
  }

  void setup() {
    watch(in_clock, [this](isa::logic_t clk) {
      if (clk) {
        _count = next_count(_count);
        next_outputs(_count);

        create_printer("Counter");
        print_info("Count: " + std::to_string(_count));
      }
    });
  }

  isa::data_t next_count(isa::data_t count) {

    if (in_reset->value()) {
      count = 0;
    } else if (in_load->value()) {
      count = in_data->value();
    } else if (in_inc->value()) {
      count++;
    }
    return count;
  }

  void next_outputs(isa::data_t count) { out_data->next(count); }

  virtual ~counter() {}
};

#endif