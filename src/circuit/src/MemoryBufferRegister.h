#ifndef MEMORY_BUFFER_REGISTER_H
#define MEMORY_BUFFER_REGISTER_H

#include "BufferedRegister.h"

class mem_buf_reg : public reg_buf
{
public:
  mem_buf_reg() {
    report_if(in_reset, "Memory Buffer Register", "Reset", &connected::truthy);
    report_if(in_load, "Memory Buffer Register", "Loaded", &connected::truthy);
    report_if(in_data, "Memory Buffer Register", "Data", &connected::truthy);
    report(out_data, "Memory Buffer Register", "Value");
  }
  virtual ~mem_buf_reg() {}
};

#endif