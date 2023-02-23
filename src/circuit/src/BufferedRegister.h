#ifndef BUFFERED_REGISTER_H

#define BUFFERED_REGISTER_H

#include "Clocked.h"
#include "Connected.h"
#include "Register.h"

class reg_buf : public reg
{
protected:
  sp<service<isa::data_t>> _buffer{0};

public:
  sp<service<isa::logic_t>> in_oe_data{0};

public:
  reg_buf() {
    _buffer = std::make_shared<service<isa::data_t>>(0);

    in_oe_data = std::make_shared<service<isa::logic_t>>(0);
    
    unwatch_all(); // rewiring?

    watch<isa::logic_t>(in_clock, [this](isa::logic_t clk) {
      if (clk) {
        if (this->in_reset->value()) {
          this->_buffer->next(0);
        } else if (this->in_load->value()) {
          this->_buffer->next(in_data->value());
        }
      }
    });

    watch_buffer(_buffer, out_data, in_oe_data);
  }
};

#endif