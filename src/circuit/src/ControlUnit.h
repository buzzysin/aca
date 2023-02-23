#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Clocked.h"
#include "Connected.h"

class control_unit : public virtual clocked, public virtual connected
{
public:
  enum state_t {
    FETCH_PC_MAR,    // enable pc
    FETCH_MAR_MEM,   // enable mar, mem
    FETCH_MEM_IR,    // enable mbr
    FETCH_IR_DECODE, // enable ir
    DECODE,          // todo (reset)
    EXECUTE,         // todo (reset)
    HALT,            // enable halt
    INIT
  };

protected:
  state_t _state = INIT;
  std::vector<sp<service<isa::logic_t>>> _outputs;

public:
  sp<service<isa::logic_t>> in_clock{0};
  sp<service<isa::data_t>> in_opcode{0};

  sp<service<isa::logic_t>> out_oe_alu{0};   // enable alu output
  sp<service<isa::logic_t>> out_oe_pc{0};    // enable pc output
  sp<service<isa::logic_t>> out_oe_ir_op{0}; // enable ir output opcode
  sp<service<isa::logic_t>> out_oe_ir_ad{0}; // enable ir output address
  sp<service<isa::logic_t>> out_oe_mem{0};   // enable mem output
  sp<service<isa::logic_t>> out_oe_mar{0};   // enable mar output
  sp<service<isa::logic_t>> out_oe_mbr{0};   // enable mbr output
  sp<service<isa::logic_t>> out_halt{0}; // enable halt output (clock will halt)

  sp<service<isa::data_t>> out_pc_mode{0};  // 0 = inc, 1 = load
  sp<service<isa::data_t>> out_alu_mode{0}; // based on opcode

  control_unit() {
    in_clock  = std::make_shared<service<isa::logic_t>>(0);
    in_opcode = std::make_shared<service<isa::data_t>>(0);

    out_oe_alu   = std::make_shared<service<isa::logic_t>>(0);
    out_oe_pc    = std::make_shared<service<isa::logic_t>>(0);
    out_oe_ir_op = std::make_shared<service<isa::logic_t>>(0);
    out_oe_ir_ad = std::make_shared<service<isa::logic_t>>(0);
    out_oe_mem   = std::make_shared<service<isa::logic_t>>(0);
    out_oe_mar   = std::make_shared<service<isa::logic_t>>(0);
    out_oe_mbr   = std::make_shared<service<isa::logic_t>>(0);
    out_halt     = std::make_shared<service<isa::logic_t>>(0);

    out_pc_mode  = std::make_shared<service<isa::data_t>>(0);
    out_alu_mode = std::make_shared<service<isa::data_t>>(0);

    _outputs = {out_oe_alu,
                out_oe_pc,
                out_oe_ir_op,
                out_oe_ir_ad,
                out_oe_mem,
                out_oe_mar,
                out_oe_mbr,
                out_halt};

    watch<isa::logic_t>(in_clock, [this](isa::logic_t clk) {
      if (clk) {
        _state = handle_state(_state);
      }
    });

    report_if(out_oe_pc, "CU", "PC Enable", &connected::truthy);
    report_if(out_oe_mar, "CU", "MAR Enable", &connected::truthy);
    report_if(out_oe_mem, "CU", "MEM Enable", &connected::truthy);
    report_if(out_oe_mbr, "CU", " MBR Enable", &connected::truthy);
    report_if(out_oe_ir_op, "CU", "IR Enable", &connected::truthy);
  }

  state_t handle_state(state_t state) {
    switch (state) {
    case FETCH_PC_MAR:
      info("CU - FETCH - loading PC into MAR...");

      set_output({out_oe_pc}, 1);
      return FETCH_MAR_MEM;

    case FETCH_MAR_MEM:
      info("CU - FETCH - selecting register on address bus...");

      set_output({out_oe_mar, out_oe_mem}, 1);
      return FETCH_MEM_IR;

    case FETCH_MEM_IR:
      info("CU - FETCH - loading instruction into IR...");

      set_output({out_oe_mbr}, 1);
      return FETCH_IR_DECODE;

    case FETCH_IR_DECODE:
      info("CU - FETCH - Todo: decode instruction...");

      set_output({}, 1);
      return DECODE;

    case DECODE:
      set_output({}, 1);
      return EXECUTE;

    case EXECUTE:
      set_output({}, 1);
      return FETCH_PC_MAR;

    case HALT:
      set_output({out_halt}, 1);
      return HALT;

    case INIT:
      info("CU - INIT - Initializing control unit...");

      set_output({}, 1);
      return FETCH_PC_MAR;

    default:
      set_output({}, 1);
      return INIT;
    }
  }

  /// @brief Sets the output of the control unit. If the service is in the out
  /// vector, it will be set to value, otherwise it will be set to !value
  /// @param out The vector of services to set to value
  /// @param value The value to set the services to
  void set_output(const std::vector<sp<service<isa::logic_t>>> &new_outputs,
                  isa::logic_t value) {
    IGNORE(value);
    bool done          = false;
    unsigned int count = new_outputs.size();

    for (sp<service<isa::logic_t>> output : _outputs) {
      for (sp<service<isa::logic_t>> new_output : new_outputs) {
        IGNORE(new_output);
        IGNORE(output);
        if (output == new_output) {
          output->next(value);
          count--;
        } else {
          output->next(!value);
        }

        done = count == 0;

        if (done) {
          break;
        }
      }
    }
  }
};

#endif