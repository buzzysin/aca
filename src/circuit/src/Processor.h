#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include "ALU.h"
#include "BufferedRegister.h"
#include "Clock.h"
#include "Connected.h"
#include "ControlUnit.h"
#include "Counter.h"
#include "InstructionRegister.h"
#include "Memory.h"
#include "MemoryAddressRegister.h"
#include "MemoryBufferRegister.h"
#include "ProgramCounter.h"
#include "Register.h"
#include "StatusRegister.h"

class address_bus;
class data_bus;

class processor : public virtual connected
{
protected:
  // Clock
  clock_ clk;

  // Control Unit
  control_unit cu;
  // ALU
  alogic_unit alu;

  // Instruction Register Opcode
  inst_reg_op ir_op;
  // Instruction Register Address (for jumps/branches)
  inst_reg_addr ir_ad;

  // Program Counter
  program_counter pc;
  // Memory Address Register
  mem_addr_reg mar;
  // Memory Buffer Register
  mem_buf_reg mbr;
  // Status Register
  reg_buf sr;
  // Memory
  memory mem{0xFF};

  // Address Bus
  sp<service<isa::data_t>> addr_bus;
  // Data Bus
  sp<service<isa::data_t>> data_bus_;

public:
  processor() {
    addr_bus  = std::make_shared<service<isa::data_t>>(0);
    data_bus_ = std::make_shared<service<isa::data_t>>(0);

    // Clock
    watch<bool>(clk.state, [this](bool state) {
      cu.in_clock->next(state);
      alu.in_clock->next(state);
      ir_op.in_clock->next(state);
      ir_ad.in_clock->next(state);
      pc.in_clock->next(state);
      mar.in_clock->next(state);
      mbr.in_clock->next(state);
      sr.in_clock->next(state);
    });

    // Control Unit
    redirect<isa::logic_t>(cu.out_oe_pc, pc.in_oe_data);
    redirect<isa::logic_t>(cu.out_oe_alu, alu.in_oe_data);
    redirect<isa::logic_t>(cu.out_oe_ir_op, ir_op.in_oe_data);
    redirect<isa::logic_t>(cu.out_oe_ir_ad, ir_ad.in_oe_data);
    redirect<isa::logic_t>(cu.out_oe_mbr, mbr.in_oe_data);
    redirect<isa::logic_t>(cu.out_oe_mar, mar.in_oe_data);

    // PC -> MAR
    redirect<isa::data_t>(pc.out_data, mar.in_data);

    // MAR -> address bus
    redirect<isa::data_t>(addr_bus, mar.in_data);

    // address bus -> Memory
    redirect<isa::data_t>(mem.addr_bus, addr_bus);

    // Memory -> Data Bus
    redirect<isa::data_t>(data_bus_, mem.data_bus);

    // ALU -> data bus
    redirect<isa::data_t>(alu.out_data, data_bus_);

    // IR (opcode) -> CU
    redirect<isa::data_t>(ir_op.out_data, cu.in_opcode);

    // IR (address) -> MAR/PC/SP
    redirect<isa::data_t>(ir_ad.out_data, mar.in_data);

    // MBR -> data bus
    redirect<isa::data_t>(mar.out_data, addr_bus);

    // MBR -> IR (opcode and address)
    redirect<isa::data_t>(mbr.out_data, ir_op.in_data);
    redirect<isa::data_t>(mbr.out_data, ir_ad.in_data);

    report(std::dynamic_pointer_cast<service<isa::data_t>>(addr_bus),
           "Address Bus",
           "Received write");
    report(std::dynamic_pointer_cast<service<isa::data_t>>(data_bus_),
           "Data Bus",
           "Received write");
  }

  virtual ~processor() {}

  void start() { clk.start(); }

  void stop() { clk.stop(); }
};

class address_bus : public virtual service<isa::addr_t>,
                    public virtual connected
{
public:
  address_bus() : service<isa::addr_t>(0) {}
  virtual ~address_bus() {}
};

class data_bus : public virtual service<isa::data_t>, public virtual connected
{
public:
  data_bus() : service<isa::addr_t>(0) {}
  virtual ~data_bus() {}
};

#endif