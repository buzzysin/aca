#ifndef STATUS_REGISTER_H
#define STATUS_REGISTER_H

#include "Register.h"

class status_reg : public reg
{
public:
  enum status {
    STATUS_ZERO,
    STATUS_NEGATIVE,
    STATUS_OVERFLOW,
    STATUS_CARRY,
  };

  status_reg() {
    report_if(in_reset, "Status Register", "Reset", &connected::truthy);
    report_if(in_load, "Status Register", "Loaded", &connected::truthy);
    report_if(in_data, "Status Register", "Data", &connected::truthy);
    report(out_data, "Status Register", "Value");
  }
  virtual ~status_reg() {}
};

#endif