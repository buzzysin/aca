#ifndef COUNTER_H
#define COUNTER_H

#include "Clocked.h"
#include "Connected.h"
#include "Register.h"

class counter : public virtual clocked, public virtual connected
{
protected:
  reg _storage;

public:
  sp<service<isa::logic_t>> in_reset{0};
  sp<service<isa::logic_t>> in_load{0};
  sp<service<isa::data_t>> in_data{0};

  sp<service<isa::data_t>> in_increment{0};

  sp<service<isa::data_t>> out_data{0};

public:
  counter() {
    in_reset = std::make_shared<service<isa::logic_t>>(0);
    in_load  = std::make_shared<service<isa::logic_t>>(0);
    in_data  = std::make_shared<service<isa::data_t>>(0);

    in_increment = std::make_shared<service<isa::data_t>>(1);

    out_data = std::make_shared<service<isa::data_t>>(0);

    redirect(in_clock, _storage.in_clock);
    redirect(in_reset, _storage.in_reset);
    redirect(in_load, _storage.in_load);
    redirect(in_data, _storage.in_data);

    redirect<isa::data_t>(_storage.out_data, out_data, [&](isa::data_t value) {
      if (in_increment->value()) {
        return value + 1;
      } else {
        return value - 1;
      }
    });

    // report_if(in_reset, "Counter - Counter reset", &connected::truthy);
    // report_if(in_load, "Counter - Counter loaded", &connected::truthy);
    // report_if(in_data, "Counter - Counter received", &connected::truthy);
    // report_if(
    //     in_increment, "Counter - Counter incremented", &connected::truthy);
    // report(out_data, "Counter - Counter value");
  }
  virtual ~counter() {}
};

#endif
