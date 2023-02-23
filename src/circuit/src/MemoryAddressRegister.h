#ifndef MEMORY_ADDRESS_REGISTER_H
#define MEMORY_ADDRESS_REGISTER_H

#include "BufferedRegister.h"

class mem_addr_reg : public reg_buf
{
public:
  mem_addr_reg() {

    report_if(in_reset, "Memory Address Register", "Reset", &connected::truthy);
    report_if(in_load, "Memory Address Register", "Loaded", &connected::truthy);
    report_if(in_data, "Memory Address Register", "Data", &connected::truthy);
    report(out_data, "Memory Address Register", "Value");
  }
  virtual ~mem_addr_reg() {}
};

#endif