#ifndef ISA_TYPES_H
#define ISA_TYPES_H

#include <cstdint>

// Set address bus size to 4 bits
#define ADDR_SIZE 4
#define ADDR_MASK 0x000F
// Registers addressible is 16
#define MEM_SIZE 16
// Data size is 8 bits
#define DATA_SIZE 8
// #define address(addr) ((isa::addr_t)addr)

namespace isa {
  typedef std::uint32_t addr_t;
  typedef std::uint32_t data_t;
  typedef bool logic_t;
} // namespace isa

#endif // ISA_TYPES_H