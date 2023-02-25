#ifndef MEMORY_ADDRESS_REGISTER_H
#define MEMORY_ADDRESS_REGISTER_H

#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Register.h"

class memory_address_register : public register_
{
public:
  memory_address_register() : register_() {

    // Just report data changes
    watch(in_data, [this](isa::data_t data) {
      create_printer("MemoryAddressRegister");
      print_info("MemoryAddressRegister received: " + std::to_string((int)data));
    });

    watch(out_data, [this](isa::data_t data) {
      create_printer("MemoryAddressRegister");
      print_info("MemoryAddressRegister sending:" + std::to_string((int)data));
    });
  }
  ~memory_address_register() {}
};

#endif