#ifndef INSTRUCTION_REGISTER_H
#define INSTRUCTION_REGISTER_H

#include <TkArch/ISA.h>

#include "AddressableRegister.h"

class instruction_register : public register_
{
public:
  instruction_register() : register_() {

    // Just report data changes
    watch(in_data, [this](isa::data_t data) {
      create_printer("Instruction Register");
      print_info("Instruction Register received: " + std::to_string((int)data));
    });

    watch(out_data, [this](isa::data_t data) {
      create_printer("Instruction Register");
      print_info("Instruction Register sending:" + std::to_string((int)data));
    });
  }
  ~instruction_register() {}
};

#endif