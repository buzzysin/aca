#ifndef INST_REG_H
#define INST_REG_H

#include "BufferedRegister.h"

class inst_reg_op : public reg_buf
{
public:
  inst_reg_op() {
    report_if(in_reset, "IR (opcode)", "Reset", &connected::truthy);
    report_if(in_load, "IR (opcode)", "Loaded", &connected::truthy);
    report_if(in_data, "IR (opcode)", "Data", &connected::truthy);
    report(out_data, "IR (opcode)", "Value");
  }

  virtual ~inst_reg_op() {}
};

class inst_reg_addr : public reg_buf
{
public:
  inst_reg_addr() {
    report_if(in_reset, "IR (address)", "Reset", &connected::truthy);
    report_if(in_load, "IR (address)", "Loaded", &connected::truthy);
    report_if(in_data, "IR (address)", "Data", &connected::truthy);
    report(out_data, "IR (address)", "Value");
  }

  virtual ~inst_reg_addr() {}
};

#endif