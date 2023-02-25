#ifndef MEMORY_H
#define MEMORY_H

#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "AddressableRegister.h"

class memory : public subscription_manager
{
protected:
  std::vector<sp<addressable_register>> _registers;

public:
  sp<signal<isa::logic_t>> in_clock;
  sp<signal<isa::logic_t>> in_reset;
  sp<signal<isa::logic_t>> in_load;
  sp<signal<isa::addr_t>> in_addr;
  sp<signal<isa::data_t>> in_data;

  sp<signal<isa::data_t>> out_data;

  isa::addr_t size{0};

public:
  memory(isa::addr_t size) : size(size) {
    in_clock = std::make_shared<signal<isa::logic_t>>(0);
    in_reset = std::make_shared<signal<isa::logic_t>>(0);
    in_load  = std::make_shared<signal<isa::logic_t>>(0);
    in_addr  = std::make_shared<signal<isa::addr_t>>(0);
    in_data  = std::make_shared<signal<isa::data_t>>(0);

    out_data = std::make_shared<signal<isa::data_t>>(0);

    setup();
  }

  void setup() {
    for (isa::addr_t i = 0; i < size; i++) {
      _registers.push_back(new_sp<addressable_register>(i));
    }

    watch(in_clock, [this](isa::logic_t clk) {
      if (clk) {
        _registers[in_addr->value()]->in_reset->next(in_reset->value());
        _registers[in_addr->value()]->in_load->next(in_load->value());
        _registers[in_addr->value()]->in_data->next(in_data->value());
        _registers[in_addr->value()]->in_clock->next(in_clock->value());

        out_data->next(_registers[in_addr->value()]->out_data->value());

        create_printer("Memory");
        print_info("Addr: " + std::to_string(in_addr->value()));
        print_info("Data: " + std::to_string(out_data->value()));
      }
    });
  }

  virtual ~memory() {}

  void write(isa::addr_t addr, isa::data_t data) {
    auto reg = _registers[addr];

    reg->in_data->next(data);
    reg->in_load->next(isa::LOGIC_HIGH);
    reg->in_clock->next(isa::LOGIC_HIGH);
    reg->in_clock->next(isa::LOGIC_LOW);
    reg->in_load->next(isa::LOGIC_LOW);
    reg->in_data->next(0);
  }
};

#endif