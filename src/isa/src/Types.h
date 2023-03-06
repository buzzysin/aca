#ifndef ISA_TYPES_H
#define ISA_TYPES_H

#include <cstdint>
#include <functional>
#include <memory>

// Set address bus size to 4 bits
#define ADDR_SIZE 4
#define ADDR_MASK 0x000F
// Registers addressible is 16
#define MEM_SIZE 16
// Data size is 8 bits
#define DATA_SIZE 8
// #define address(addr) ((isa::addr_t)addr)

namespace isa {
  typedef std::uint8_t addr_t;
  typedef std::size_t data_t;
  struct logic_t {
    int value;
    static const logic_t LOGIC_LOW, LOGIC_HIGH, LOGIC_Z;
    constexpr logic_t(int value = 1) : value(value) {}
    // constexpr operator int() const { return value; }
    constexpr operator bool() const { return value == 2; }
  };
  constexpr const logic_t logic_t::LOGIC_LOW  = 0;
  constexpr const logic_t logic_t::LOGIC_Z    = 1;
  constexpr const logic_t logic_t::LOGIC_HIGH = 2;

  constexpr const logic_t LOGIC_LOW  = logic_t::LOGIC_LOW;
  constexpr const logic_t LOGIC_Z    = logic_t::LOGIC_Z;
  constexpr const logic_t LOGIC_HIGH = logic_t::LOGIC_HIGH;

  static inline logic_t not_logic(logic_t a) {
    // 2 - 0 = 2
    // 2 - 1 = 1
    // 2 - 2 = 0
    return logic_t(2 - a.value);
  }

} // namespace isa

#endif // ISA_TYPES_H