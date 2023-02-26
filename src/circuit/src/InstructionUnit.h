#ifndef INSTRUCTION_UNIT
#define INSTRUCTION_UNIT

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Helpers.h"

class instruction_unit : public subscription_manager, public signal_dispatcher
{
public:
  enum state_t {
    INIT,
    WAIT,
    FETCH_PC_MAR,    // enable pc
    FETCH_MAR_MEM,   // enable mar, mem
    FETCH_MBR_IR,    // enable mbr
    FETCH_IR_DECODE, // enable ir
  };

protected:
  state_t _state = WAIT;

public:
  sp<signal<isa::logic_t>> out_pc;
  sp<signal<isa::logic_t>> out_pc_clk;
  sp<signal<isa::logic_t>> out_pc_inc;

  sp<signal<isa::logic_t>> out_mar;
  sp<signal<isa::logic_t>> out_mar_clk;
  sp<signal<isa::logic_t>> out_mar_load;

  sp<signal<isa::logic_t>> out_mem;
  sp<signal<isa::logic_t>> out_mem_clk;
  sp<signal<isa::logic_t>> out_mem_load;

  sp<signal<isa::logic_t>> out_mbr;
  sp<signal<isa::logic_t>> out_mbr_clk;
  sp<signal<isa::logic_t>> out_mbr_load;

  sp<signal<isa::logic_t>> out_ir;
  sp<signal<isa::logic_t>> out_ir_clk;
  sp<signal<isa::logic_t>> out_ir_load;

  sp<signal<isa::logic_t>> out_halt;

  sp<signal<isa::logic_t>> in_clock;
  sp<signal<isa::data_t>> in_ir_decode;

  sp<signal<isa::logic_t>> in_fetch;
  sp<signal<isa::logic_t>> out_done;

public:
  instruction_unit() {
    out_pc     = new_sp<signal<isa::logic_t>>(0);
    out_pc_clk = new_sp<signal<isa::logic_t>>(0);
    out_pc_inc = new_sp<signal<isa::logic_t>>(0);

    out_mar      = new_sp<signal<isa::logic_t>>(0);
    out_mar_clk  = new_sp<signal<isa::logic_t>>(0);
    out_mar_load = new_sp<signal<isa::logic_t>>(0);

    out_mem      = new_sp<signal<isa::logic_t>>(0);
    out_mem_clk  = new_sp<signal<isa::logic_t>>(0);
    out_mem_load = new_sp<signal<isa::logic_t>>(0);

    out_mbr      = new_sp<signal<isa::logic_t>>(0);
    out_mbr_clk  = new_sp<signal<isa::logic_t>>(0);
    out_mbr_load = new_sp<signal<isa::logic_t>>(0);

    out_ir      = new_sp<signal<isa::logic_t>>(0);
    out_ir_clk  = new_sp<signal<isa::logic_t>>(0);
    out_ir_load = new_sp<signal<isa::logic_t>>(0);

    out_halt = new_sp<signal<isa::logic_t>>(0);

    in_clock     = new_sp<signal<isa::logic_t>>(0);
    in_ir_decode = new_sp<signal<isa::data_t>>(0);

    in_fetch = new_sp<signal<isa::logic_t>>(0);
    out_done = new_sp<signal<isa::logic_t>>(0);

    out_signals_data = {out_pc,
                        out_pc_inc,
                        out_mar,
                        out_mar_load,
                        out_mem,
                        out_mem_load,
                        out_mbr,
                        out_mbr_load,
                        out_ir,
                        out_ir_load,
                        out_halt};

    out_signals_clk = {
        out_pc_clk, out_mar_clk, out_mem_clk, out_mbr_clk, out_ir_clk};

    setup();
  }

  ~instruction_unit() {}

  void setup() {

    watch(in_clock, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        _state = next_state(_state);
        next_outputs(_state);
      }
    });

    watch(in_ir_decode, [this](const isa::data_t &value) {
      create_printer("Instruction Unit");
      print_info("Received IR Decode Signal: " + std::to_string((int)value));
    });
  }

  state_t next_state(state_t state) {
    bool fetch = in_fetch->value() == isa::LOGIC_HIGH;
    switch (state) {
    case WAIT:
      if (fetch) {
        return FETCH_PC_MAR;
      } else {
        return WAIT;
      }
    case FETCH_PC_MAR:
      return FETCH_MAR_MEM;
    case FETCH_MAR_MEM:
      return FETCH_MBR_IR;
    case FETCH_MBR_IR:
      return FETCH_IR_DECODE;
    case FETCH_IR_DECODE:
      return WAIT;
    case INIT:
      return WAIT;
    default:
      return WAIT;
    }
  }

  void next_outputs(state_t state) {
    create_printer("Instruction Unit");

    switch (state) {
    case WAIT:
      print_info("Initialising Instruction Unit");
      set_outputs({});
      break;

    case FETCH_PC_MAR:
      print_info("FETCH - PC into MAR");
      // output the pc, load the mar, propogate output
      set_outputs({out_pc, out_mar_load, out_mar_clk, out_mar});
      break;

    case FETCH_MAR_MEM:
      print_info("FETCH - MAR into MEM");
      // read memory to bus, propogate output
      set_outputs({out_mem, out_mem_clk, out_mbr_load, out_mbr_clk});
      break;

    case FETCH_MBR_IR:
      print_info("FETCH - MBR into IR");
      // output the bus, load the ir, propogate output
      set_outputs({out_mbr, out_ir_load, out_ir_clk});
      break;

    case FETCH_IR_DECODE:
      print_info("FETCH - IR into DECODE");
      // output the ir into the decoder, increment the pc
      set_outputs({out_ir, out_pc_inc, out_pc_clk});
      break;

    default:
      break;
    }
  }
};

#endif