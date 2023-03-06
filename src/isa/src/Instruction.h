#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "Types.h"

#include <bitset>
#include <stdexcept>

namespace isa {
  static constexpr unsigned int ceil_log2(unsigned int n) {
    unsigned int p   = 1;
    unsigned int exp = 0;
    while (p < n) {
      p *= 2;
      exp++;
    }
    return exp;
  }

  static constexpr unsigned int round_pow2(unsigned int n) {
    unsigned int p = 1;
    while (p < n) {
      p *= 2;
    }
    return p;
  }

  struct inst_opcode {
    unsigned int value;

    constexpr inst_opcode(unsigned int value = 0) : value(value) {}
    bool operator==(const inst_opcode &other) const {
      return value == other.value;
    }
    bool operator!=(const inst_opcode &other) const {
      return value != other.value;
    }

    std::string to_string() const {
      switch (value) {
      case 0:
        return "HLT";
      case 1:
        return "LDR";
      case 2:
        return "STR";
      case 3:
        return "MOV";
      case 4:
        return "ADD";
      case 5:
        return "SUB";
      case 6:
        return "AND";
      case 7:
        return "ORR";
      case 8:
        return "NOT";
      case 9:
        return "CMP";
      case 10:
        return "JMP";
      case 11:
        return "B";
      default:
        return "UNKNOWN";
      }
    }
  };

  static const inst_opcode HLT = 0;  // operands: 0
  static const inst_opcode LDR = 1;  // operands: 1
  static const inst_opcode STR = 2;  // operands: 1
  static const inst_opcode MOV = 3;  // operands: 1
  static const inst_opcode ADD = 4;  // operands: 3
  static const inst_opcode SUB = 5;  // operands: 3
  static const inst_opcode AND = 6;  // operands: 3
  static const inst_opcode ORR = 7;  // operands: 3
  static const inst_opcode NOT = 8;  // operands: 2
  static const inst_opcode CMP = 9;  // operands: 2
  static const inst_opcode JMP = 10; // operands: 1
  static const inst_opcode B   = 11; // operands: 1

  static const unsigned int N_OPCODES     = 12;
  static const unsigned int N_OPCODE_BITS = ceil_log2(round_pow2(N_OPCODES));

  enum inst_cond { NO, EQ, NE, GT, LT, GE, LE, N_CONDS };

  std::vector<std::string> inst_cond_names = {
      "NO", "EQ", "NE", "GT", "LT", "GE", "LE"};

  struct inst_mode {
    unsigned int value;
    constexpr inst_mode(unsigned int value = 0) : value(value) {}
    bool operator==(const inst_mode &other) const {
      return value == other.value;
    }
    bool operator!=(const inst_mode &other) const {
      return value != other.value;
    }
  };

  static const inst_mode UNDEFINED = 0;
  static const inst_mode IMM       = 0;
  static const inst_mode REG       = 1;

  struct inst_operand {
    inst_mode mode;
    unsigned int value;
    inst_operand() : mode(UNDEFINED), value(0) {}
    inst_operand(unsigned int value) : mode(IMM), value(value) {}
    inst_operand(inst_mode mode, data_t value) : mode(mode), value(value) {}

    std::string to_string() const {
      if (mode == UNDEFINED) {
        return "";
      } else if (mode == IMM) {
        return "#" + std::to_string(value);
      } else if (mode == REG) {
        return "R" + std::to_string(value);
      } else {
        throw std::runtime_error("Invalid operand mode");
      }
    }
  };

  static inst_operand reg(data_t value) { return inst_operand(REG, value); }

  static inst_operand imm(data_t value) { return inst_operand(IMM, value); }

  static inst_operand empty() { return inst_operand(); }

  static const inst_operand R0 = reg(0);
  static const inst_operand R1 = reg(1);
  static const inst_operand R2 = reg(2);
  static const inst_operand R3 = reg(3);
  static const inst_operand R4 = reg(4);
  static const inst_operand R5 = reg(5);
  static const inst_operand R6 = reg(6);
  static const inst_operand R7 = reg(7);

  struct instruction {
    inst_opcode opcode    = HLT;
    inst_cond condition   = NO;
    inst_operand operand1 = empty();
    inst_operand operand2 = empty();
    inst_operand operand3 = empty();

    instruction() {}
    instruction(data_t data) {
      std::cout << "data: " << std::bitset<16>(data) << std::endl;

      // bits 15-12: opcode
      opcode = inst_opcode((data >> 12) & 0b1111);

      // bits 11-9: operand 1
      operand1 = inst_operand(inst_mode(REG), (data >> 9) & 0b111);

      // bits 8-5: operand 2
      operand2 =
          inst_operand(inst_mode((data >> 8) & 0b1), (data >> 5) & 0b111);

      if ((opcode != MOV) && (opcode != LDR) && (opcode != STR)) {
        // bits 4-0: operand 3
        operand3 =
            inst_operand(inst_mode((data >> 4) & 0b1), (data >> 1) & 0b111);
      }
    }
    instruction(inst_opcode opcode,
                inst_cond condition,
                inst_operand operand1,
                inst_operand operand2)
        : opcode(opcode), condition(condition), operand1(operand1),
          operand2(operand2) {}

    friend inline instruction operator|(instruction inst,
                                        const inst_cond &cond) {
      inst.condition = cond;
      return inst;
    }

    friend inline instruction operator|(instruction inst,
                                        const inst_operand &op) {

      if (inst.operand1.mode == UNDEFINED) {
        if (op.mode == IMM && (inst.opcode != JMP && inst.opcode != B)) {
          throw std::runtime_error("Immediate value cannot be first operand");
        } else {
          inst.operand1 = op;
        }
      } else if (inst.operand2.mode == UNDEFINED) {
        inst.operand2 = op;
      } else if (inst.operand3.mode == UNDEFINED) {
        inst.operand3 = op;
      } else {
        throw std::runtime_error("Too many operands");
      }

      return inst;
    }

    friend inline instruction operator|(instruction inst,
                                        const inst_opcode &op) {
      inst.opcode = op;
      return inst;
    }

    unsigned int to_uint() const {
      unsigned int repr = 0;

      repr <<= 4;
      repr |= opcode.value & 0b1111;

      repr <<= 3;
      repr |= operand1.value & 0b111;

      repr <<= 1;
      repr |= operand2.mode.value & 0b1;

      repr <<= 3;
      repr |= operand2.value & 0b111;

      repr <<= 1;
      repr |= operand3.mode.value & 0b1;

      repr <<= 3;
      repr |= operand3.value & 0b111;

      repr <<= 1;
      repr |= 0; // extra bit

      return repr;
    }

    std::string to_string() const {
      std::string repr = "";

      repr += opcode.to_string() + " ";
      repr += operand1.to_string() + " ";
      repr += operand2.to_string() + " ";
      repr += operand3.to_string();

      return repr;
    }

    std::string to_bin_string() const {
      std::string repr = "";

      repr += std::bitset<4>(opcode.value).to_string() + " ";
      repr += std::bitset<3>(operand1.value).to_string() + " ";
      repr += std::bitset<1>(operand2.mode.value).to_string() + " ";
      repr += std::bitset<3>(operand2.value).to_string() + " ";
      repr += std::bitset<1>(operand3.mode.value).to_string() + " ";
      repr += std::bitset<3>(operand3.value).to_string() + " ";
      repr += std::bitset<1>(0).to_string() + " "; // Padding

      return repr;
    }
  };

  static std::vector<instruction> prog_example = {
      // Start
      /* 00 */ instruction() | MOV | R0 | imm(3),  // R0 = 3
      /* 01 */ instruction() | MOV | R1 | imm(4),  // R1 = 4
      /* 02 */ instruction() | ADD | R2 | R0 | R1, // R2 = R0 + R1

      // End
      /* 03 */ instruction() | HLT,
  };

  // Example program:
  static std::vector<instruction> prog_tri_10 = prog_tri_10 = {
      // Start
      /* 00 */ instruction() | MOV | R0 | imm(0), // R0 <- 0
      /* 01 */ instruction() | MOV | R1 | imm(0), // R1 <- 0

      // Loop
      /* 02 */ instruction() | CMP | R0 | imm(10), // R0 == 10
      /* 03 */ instruction() | B | EQ | imm(7),    // if (R0 == 10) goto 7
      /* 04 */ instruction() | ADD | R1 | R1 | R0, // R1 <- R1 + R0
      /* 05 */ instruction() | ADD | R0 | R0 | R1, // R0 <- R0 + R1
      /* 06 */ instruction() | JMP | imm(2),       // goto 2

      // End
      /* 07 */ instruction() | HLT,
  };

  static std::vector<instruction> prog_quicksort = {
    // Start
    /* 00 */ instruction() | MOV | R0 | imm(1), // R0 <- 0
    /* 01 */ instruction() | MOV | R1 | imm(2), // R1 <- 0
    /* 02 */ instruction() | MOV | R2 | imm(3), // R2 <- 0
    /* 03 */ instruction() | MOV | R3 | imm(4), // R3 <- 0
    /* 04 */ instruction() | MOV | R4 | imm(5), // R4 <- 0
    /* 05 */ instruction() | STR | R0 | imm(3),
    /* 06 */ instruction() | STR | R1 | imm(1),
    /* 07 */ instruction() | STR | R2 | imm(0),
    /* 08 */ instruction() | STR | R3 | imm(4),
    /* 09 */ instruction() | STR | R4 | imm(2),
  };
} // namespace isa

#endif