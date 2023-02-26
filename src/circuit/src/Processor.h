#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <TkArch/Debug.h>
#include <TkArch/Signals.h>

#include "Clock.h"
#include "ControlUnit.h"
#include "InstructionRegister.h"
#include "Memory.h"
#include "MemoryAddressRegister.h"
#include "MemoryBufferRegister.h"
#include "ProgramCounter.h"

class processor : public subscription_manager
{
public:
  clock_ clk;
  control_unit cu;
  program_counter pc;
  memory mem{0x10};
  memory_address_register mar;
  memory_buffer_register mbr;
  instruction_register ir;

  sp<signal<isa::addr_t>> addr_bus;
  sp<signal<isa::data_t>> data_bus;

protected:
  sp<signal<isa::data_t>> out_pc_buf;
  sp<signal<isa::data_t>> out_mem_buf;
  sp<signal<isa::data_t>> out_mar_buf;
  sp<signal<isa::data_t>> out_mbr_buf;
  sp<signal<isa::data_t>> out_ir_buf;

public:
  processor() {
    addr_bus = new_sp<signal<isa::addr_t>>(0);
    data_bus = new_sp<signal<isa::data_t>>(0);

    out_pc_buf  = new_sp<signal<isa::data_t>>(0);
    out_mem_buf = new_sp<signal<isa::data_t>>(0);
    out_mar_buf = new_sp<signal<isa::data_t>>(0);
    out_mbr_buf = new_sp<signal<isa::data_t>>(0);
    out_ir_buf  = new_sp<signal<isa::data_t>>(0);

    setup();
  }

  void start() { clk.start(); }
  void stop() { clk.stop(); }
  void halt() { clk.halt(); }
  void advance() {
    clk.tick();
    clk.tick();
  }
  ~processor() {}

protected:
  void setup() {
    // Connect clock
    watch(clk.state, cu.in_clock);

    // Program counter (PC)
    // Clock the PC
    watch(cu.out_pc_clk, pc.count.in_clock);
    // Increment the PC
    watch(cu.out_pc_inc, pc.count.in_inc);
    // Write the PC output buffer
    watch(pc.out_data, out_pc_buf);
    // Write the PC output buffer to the MAR
    watch(out_pc_buf, mar.in_data, cu.out_pc);

    // Memory address register (MAR)
    // Clock the MAR
    watch(cu.out_mar_clk, mar.in_clock);
    watch(cu.out_mar_load, mar.in_load);
    // Write the MAR output to buffer
    watch(mar.out_data, out_mar_buf, cu.out_mar);
    // Write the MAR output buffer to memory address
    watch(out_mar_buf, mem.in_addr, cu.out_mar);

    // Memory
    // Clock the memory
    watch(cu.out_mem_clk, mem.in_clock);
    watch(cu.out_mem_load, mem.in_load);
    // Write the memory output to buffer
    watch(mem.out_data, out_mem_buf, cu.out_mem);
    // Write the memory output buffer to MBR
    watch(out_mem_buf, mbr.in_data, cu.out_mem);

    // Memory buffer register (MBR)
    // Clock the MBR
    watch(cu.out_mbr_clk, mbr.in_clock);
    watch(cu.out_mbr_load, mbr.in_load);
    // Write the MBR output to buffer
    watch(mbr.out_data, out_mbr_buf, cu.out_mbr);
    // Write the MBR output buffer to memory
    watch(out_mbr_buf, mem.in_data, cu.out_mbr);
    // Write the MBR output buffer to IR
    watch(out_mbr_buf, ir.in_data, cu.out_mbr);

    // Instruction register (IR)
    // Clock the IR
    watch(cu.out_ir_clk, ir.in_clock);
    watch(cu.out_ir_load, ir.in_load);
    // Write the IR output to buffer
    watch(ir.out_data, out_ir_buf, cu.out_ir);
    // Write the IR output buffer to program counter
    watch(out_ir_buf, pc.count.in_data, cu.out_ir);
    // Write the IR output buffer to control unit
    watch(out_ir_buf, cu.in_ir_decode, cu.out_ir);

    // watch(mem.out_data, );
  }
};

#endif