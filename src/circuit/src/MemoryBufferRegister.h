#ifndef MEMORY_BUFFER_REGISTER_H
#define MEMORY_BUFFER_REGISTER_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Register.h"

class memory_buffer_register : public register_
{
public:
  memory_buffer_register() : register_() {
    // Just report data changes
    watch(in_data, [this](isa::data_t data) {
      create_printer("MemoryBufferRegister");
      print_info("MemoryBufferRegister received: " + std::to_string((int)data));
    });

    watch(out_data, [this](isa::data_t data) {
      create_printer("MemoryBufferRegister");
      print_info("MemoryBufferRegister sending: " + std::to_string((int)data));
    });
  }
  ~memory_buffer_register() {}
};

#endif