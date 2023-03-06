#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>
#include <TkArch/Types.h>

#include <vector>

#include "ExecutionUnit.h"
#include "Helpers.h"
#include "InstructionUnit.h"

class control_unit : public subscription_manager, public signal_dispatcher
{
public:
  enum state_t {
    HALT, // enable halt
    FETCH,
    DECODE,
    EXECUTE,
    INIT
  };

  std::vector<std::string> state_names = {"HALT",
                                          "FETCH_PC_MAR",
                                          "FETCH_MAR_MEM",
                                          "FETCH_MBR_IR",
                                          "FETCH_IR_DECODE",
                                          "DECODE",
                                          "EXECUTE",
                                          "INIT"};

protected:
  state_t _state = INIT;

  instruction_unit iu;
  execution_unit eu;

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

public:
  control_unit() {
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

    setup();
  }

  void setup() {
    setup_instruction_unit();
    setup_execution_unit();

    watch(in_clock, [this](const isa::logic_t& value) {
      if (value) {
        _state = next_state(_state);
        next_outputs(_state);
      }
    });
  }

  void setup_instruction_unit() {
    watch(in_clock, iu.in_clock);
    watch(in_ir_decode, iu.in_ir_decode);

    watch(iu.out_pc, out_pc);
    watch(iu.out_pc_clk, out_pc_clk);
    watch(iu.out_pc_inc, out_pc_inc);

    watch(iu.out_mar, out_mar);
    watch(iu.out_mar_clk, out_mar_clk);
    watch(iu.out_mar_load, out_mar_load);

    watch(iu.out_mem, out_mem);
    watch(iu.out_mem_clk, out_mem_clk);
    watch(iu.out_mem_load, out_mem_load);

    watch(iu.out_mbr, out_mbr);
    watch(iu.out_mbr_clk, out_mbr_clk);
    watch(iu.out_mbr_load, out_mbr_load);

    watch(iu.out_ir, out_ir);
    watch(iu.out_ir_clk, out_ir_clk);
    watch(iu.out_ir_load, out_ir_load);

    watch(iu.out_halt, out_halt);
  }

  void setup_execution_unit() {}

  state_t next_state(state_t state) {
    isa::logic_t fetch_done   = iu.out_done->value();
    isa::logic_t execute_done = eu.out_done->value();

    switch (state) {
    case INIT:
      return FETCH;

    case FETCH:
      if (fetch_done) {
        return DECODE;
      } else {
        return FETCH;
      }

    case DECODE:
      return EXECUTE;

    case EXECUTE:
      if (execute_done) {
        return FETCH;
      } else {
        return EXECUTE;
      }

    case HALT:
      return HALT;

    default:
      return FETCH;
    }
  }

  void next_outputs(state_t state) {
    switch (state) {
    case INIT:
      iu.in_fetch->next(isa::LOGIC_LOW);
      eu.in_execute->next(isa::LOGIC_LOW);
      break;

    case FETCH:
      iu.in_fetch->next(isa::LOGIC_HIGH);
      break;

    case DECODE:
      iu.in_fetch->next(isa::LOGIC_LOW);
      break;

    case EXECUTE:
      eu.in_execute->next(isa::LOGIC_HIGH);
      break;

    case HALT:
      iu.in_fetch->next(isa::LOGIC_LOW);
      eu.in_execute->next(isa::LOGIC_LOW);
      break;

    default:
      iu.in_fetch->next(isa::LOGIC_LOW);
      eu.in_execute->next(isa::LOGIC_LOW);
      break;
    }
  }
};

#endif // CONTROL_UNIT_H