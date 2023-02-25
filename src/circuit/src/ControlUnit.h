#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>
#include <TkArch/Types.h>

class control_unit : public subscription_manager
{
public:
  enum state_t {
    HALT,            // enable halt
    FETCH_PC_MAR,    // enable pc
    FETCH_MAR_MEM,   // enable mar, mem
    FETCH_MBR_IR,    // enable mbr
    FETCH_IR_DECODE, // enable ir
    DECODE,          // todo (halt)
    EXECUTE,         // todo (halt)
    INIT             // inital state
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
  state_t _state = INIT, _next_state = INIT;

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

  std::vector<sp<signal<isa::logic_t>>> out_signals;

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

    in_clock = new_sp<signal<isa::logic_t>>(0);

    out_signals = {out_pc,
                   out_pc_inc,
                   out_pc_clk,
                   out_mar,
                   out_mar_load,
                   out_mar_clk,
                   out_mem,
                   out_mem_load,
                   out_mem_clk,
                   out_mbr,
                   out_mbr_load,
                   out_mbr_clk,
                   out_ir,
                   out_ir_load,
                   out_ir_clk,
                   out_halt};

    setup();
  }

  void set_outputs(const std::vector<sp<signal<isa::logic_t>>> &outputs,
                   isa::logic_t value = isa::LOGIC_HIGH) {
    std::vector<sp<signal<isa::logic_t>>> nots = {};

    for (auto &output : out_signals) {
      for (auto &out : outputs) {
        if (output == out) {
          output->next(value);
        } else {
          nots.push_back(output);
        }
      }
    }

    for (auto &not_out : nots) {
      not_out->next(isa::not_logic(value));
    }
  }

  void setup() {

    watch(in_clock, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        _state = next_state(_state);
        next_outputs(_state);

        create_printer("Control Unit");
        print_info("State: " + state_names[_state]);
      }
    });

    watch(out_pc, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("PC: " + std::to_string(out_pc->value()));
      }
    });

    watch(out_pc_clk, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("PC CLK: " + std::to_string(out_pc_clk->value()));
      }
    });

    watch(out_pc_inc, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("PC INC: " + std::to_string(out_pc_inc->value()));
      }
    });

    watch(out_mar, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("MAR: " + std::to_string(out_mar->value()));
      }
    });

    watch(out_mar_clk, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("MAR CLK: " + std::to_string(out_mar_clk->value()));
      }
    });

    watch(out_mem, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("MEM: " + std::to_string(out_mem->value()));
      }
    });

    watch(out_mem_clk, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("MEM CLK: " + std::to_string(out_mem_clk->value()));
      }
    });

    watch(out_mbr, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("MBR: " + std::to_string(out_mbr->value()));
      }
    });

    watch(out_mbr_clk, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("MBR CLK: " + std::to_string(out_mbr_clk->value()));
      }
    });

    watch(out_ir, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("IR: " + std::to_string(out_ir->value()));
      }
    });

    watch(out_ir_clk, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("IR CLK: " + std::to_string(out_ir_clk->value()));
      }
    });

    watch(out_halt, [this](const isa::logic_t &value) {
      if (value == isa::LOGIC_HIGH) {
        create_printer("Control Unit");
        print_info("HALT");
      }
    });
  }

  state_t next_state(state_t state) {
    switch (state) {
    case INIT:
      return FETCH_PC_MAR;
    case FETCH_PC_MAR:
      return FETCH_MAR_MEM;
    case FETCH_MAR_MEM:
      return FETCH_MBR_IR;
    case FETCH_MBR_IR:
      return FETCH_IR_DECODE;
    case FETCH_IR_DECODE:
      return DECODE;
    case DECODE:
      return EXECUTE;
    case EXECUTE:
      return FETCH_PC_MAR;
    case HALT:
      return HALT;
    default:
      return INIT;
    }
  }

  void next_outputs(state_t state) {
    create_printer("Control Unit");
    switch (state) {
    case INIT:
      print_info("Initialising Control Unit");
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

    case DECODE:
      set_outputs({});
      break;

    case EXECUTE:
      set_outputs({});
      break;

    case HALT:
      set_outputs({out_halt});
      break;

    default:
      set_outputs({});
      break;
    }
  }
};

#endif // CONTROL_UNIT_H