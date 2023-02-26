#ifndef EXECUTION_UNIT_H
#define EXECUTION_UNIT_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "Helpers.h"

class execution_unit : public subscription_manager, public signal_dispatcher
{
public:
  enum state_t {
    INIT,
  };

protected:
  state_t _state = INIT;

public:
  // sp<signal<isa::logic_t>> out_pc;
  // sp<signal<isa::logic_t>> out_pc_clk;
  // sp<signal<isa::logic_t>> out_pc_inc;

  // sp<signal<isa::logic_t>> out_mar;
  // sp<signal<isa::logic_t>> out_mar_clk;
  // sp<signal<isa::logic_t>> out_mar_load;

  // sp<signal<isa::logic_t>> out_mem;
  // sp<signal<isa::logic_t>> out_mem_clk;
  // sp<signal<isa::logic_t>> out_mem_load;

  // sp<signal<isa::logic_t>> out_mbr;
  // sp<signal<isa::logic_t>> out_mbr_clk;
  // sp<signal<isa::logic_t>> out_mbr_load;

  // sp<signal<isa::logic_t>> out_ir;
  // sp<signal<isa::logic_t>> out_ir_clk;
  // sp<signal<isa::logic_t>> out_ir_load;

  sp<signal<isa::logic_t>> out_halt;

  sp<signal<isa::logic_t>> in_clock;

  sp<signal<isa::logic_t>> in_execute;
  sp<signal<isa::logic_t>> out_done;

public:
  execution_unit() {
    out_halt = new_sp<signal<isa::logic_t>>(0);

    in_clock = new_sp<signal<isa::logic_t>>(0);

    in_execute = new_sp<signal<isa::logic_t>>(0);
    out_done = new_sp<signal<isa::logic_t>>(0);
  }
};

#endif