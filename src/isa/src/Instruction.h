#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "Types.h"

#include <stdexcept>

namespace isa {
  enum inst_opcode {
    // Halt
    HLT,
    // Retrieval
    LDR,
    // Store
    STR,
    MOV,
    // Arithmetic
    ADD,
    SUB,
    AND,
    ORR,
    NOT,
    // Control
    CMP,
    JMP,
    B,
  };

  enum inst_cond { NO, EQ, NE, GT, LT, GE, LE };

  struct inst_mode {
    static inst_mode EMPTY;
    static inst_mode IMM;
    static inst_mode REG;
    unsigned int value;
    constexpr inst_mode() : value(0) {}
    constexpr inst_mode(unsigned int value) : value(value) {}
    friend inline bool operator==(inst_mode a, inst_mode b) {
      return a.value == b.value;
    }
  };

  inst_mode inst_mode::EMPTY = 0;
  inst_mode inst_mode::IMM   = 1;
  inst_mode inst_mode::REG   = 2;

  struct inst_operand {
    inst_mode mode;
    data_t value;
    inst_operand() : mode(inst_mode::EMPTY), value(0) {}
    inst_operand(unsigned int value) : mode(inst_mode::IMM), value(value) {}
    inst_operand(inst_mode mode, data_t value) : mode(mode), value(value) {}
  };

  static inst_operand reg(data_t value) {
    return inst_operand(inst_mode::REG, value);
  }

  static inst_operand imm(data_t value) {
    return inst_operand(inst_mode::IMM, value);
  }

  static inst_operand empty() { return inst_operand(); }

  static inst_operand R0 = reg(0);
  static inst_operand R1 = reg(1);
  static inst_operand R2 = reg(2);
  static inst_operand R3 = reg(3);
  static inst_operand R4 = reg(4);
  static inst_operand R5 = reg(5);
  static inst_operand R6 = reg(6);
  static inst_operand R7 = reg(7);

  struct instruction {
    inst_opcode opcode    = HLT;
    inst_cond condition   = NO;
    inst_operand operand1 = empty();
    inst_operand operand2 = empty();
    inst_operand operand3 = empty();
    instruction() : opcode(HLT), condition(NO) {}
    instruction(inst_opcode opcode,
                inst_cond condition,
                inst_operand operand1,
                inst_operand operand2)
        : opcode(opcode), condition(condition), operand1(operand1),
          operand2(operand2) {}

    friend inline instruction operator|(instruction inst, inst_cond cond) {
      inst.condition = cond;
      return inst;
    }

    friend inline instruction operator|(instruction inst, inst_operand op) {
      if (inst.operand1.mode == inst_mode::EMPTY) {
        if (op.mode == inst_mode::IMM &&
            (inst.opcode != JMP && inst.opcode != B)) {
          throw std::runtime_error("Immediate value cannot be first operand");
        } else {
          inst.operand1 = op;
        }
      } else if (inst.operand2.mode == inst_mode::EMPTY) {
        inst.operand2 = op;
      } else if (inst.operand3.mode == inst_mode::EMPTY) {
        inst.operand3 = op;
      } else {
        throw std::runtime_error("Too many operands");
      }
      return inst;
    }

    friend inline instruction operator|(instruction inst, inst_opcode op) {
      inst.opcode = op;
      return inst;
    }

    unsigned int to_uint() const {
      unsigned int repr = 0;

      // repr = opcode | operand1 | operand2 | operand3
      // we will do conditional later

      repr <<= 4, repr |= opcode;
      repr <<= 3, repr |= operand1.value;
      repr <<= 1, repr |= operand2.mode.value;
      repr <<= 3, repr |= operand2.value;
      repr <<= 1, repr |= operand3.mode.value;
      repr <<= 3, repr |= operand3.value;
      repr <<= 1, repr |= 0; // Padding

      return repr;
    }
  };

  // Example program:
  static std::vector<instruction> prog_tri_10 = prog_tri_10 = {
      /* 00 */ instruction() | MOV | R0 | imm(0), // Start
      /* 01 */ instruction() | MOV | R1 | imm(0),
      /* 02 */ instruction() | CMP | R0 | imm(10), // Loop
      /* 03 */ instruction() | B | EQ | imm(7),
      /* 04 */ instruction() | ADD | R1 | R1 | R0,
      /* 05 */ instruction() | ADD | R0 | R0 | R1,
      /* 06 */ instruction() | JMP | imm(2),
      /* 07 */ instruction() | HLT, // End
  };

  static std::vector<instruction> prog_quicksort = {
      /* 00 */ instruction() | HLT // TODO
  };
} // namespace isa

#endif