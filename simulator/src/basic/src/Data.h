#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define DATA_BIT_WIDTH 8 // 8 bits to store values up to 255

#define ADDR_MODES 2     // 2 select modes
#define ADDR_BIT_WIDTH 4 // 4 bits to select 16 registers

#define INST_MODE_WIDTH 1 // 1 bit to select 2 modes, may increase
#define INST_OPCD_WIDTH 5 // 5 bits to select 32 instructions

typedef unsigned char data_t;

enum InstReg : data_t { R0, R1, R2, R3, R4, R5, R6, R7, REG_COUNT };

enum InstAddrMode : data_t {
  REGISTER,  // OP2/OP3 is a register
  IMMEDIATE, // OP2/OP3 is a value
  MODE_COUNT
};

enum InstOpCode : data_t {
  // Data management (2 arguments)
  LDX, // Load Register
  STX, // Store Register
  MOV, // Move

  // Arithmetic (3 arguments)
  ADD, // Add
  SUB, // Subtract
  MUL, // Multiply
  DIV, // Divide
  MOD, // Modulo

  // Bitwise (2 arguments)
  AND, // Bitwise AND
  ORR, // Bitwise OR
  XOR, // Bitwise XOR
  // Bitwise (Unary)
  NOT, // Bitwise NOT
  SHL, // Shift Left
  SHR, // Shift Right

  // Logic (2 arguments)
  CMP, // Compare

  // Control (Unary)
  JMP, // Jump
  BEQ, // Branch if Equal
  BNE, // Branch if Not Equal
  BGT, // Branch if Greater Than
  BLT, // Branch if Less Than
  BGE, // Branch if Greater Than or Equal
  BLE, // Branch if Less Than or Equal
  // Control (Unitary)
  HLT, // Halt
  NOP, // No Operation
  INST_OP_COUNT
};

class Instruction
{
  data_t data : DATA_BIT_WIDTH{0};

public:
  Instruction() { this->data = 0; }
  Instruction(data_t data) { this->data = data; }
  Instruction(InstOpCode opCode, InstAddrMode mode, InstReg op1, InstReg op2,
              InstReg op3) {
    this->data = 0;
  }
};

// This represents an unary instruction.
// It has only one operand. Given this,
// we have
struct UnInst : public Instruction {
};

#endif