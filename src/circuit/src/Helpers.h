#ifndef HELPERS_H
#define HELPERS_H

#include <vector>

#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

class signal_dispatcher
{
protected:
  std::vector<sp<signal<isa::logic_t>>> out_signals_data;
  std::vector<sp<signal<isa::logic_t>>> out_signals_clk;

protected:
  void set_outputs(const std::vector<sp<signal<isa::logic_t>>> &outputs,
                   isa::logic_t value = isa::LOGIC_HIGH) {
    // Do the data phase first
    for (auto &output : out_signals_data) {
      for (auto &out : outputs) {
        if (output == out) {
          output->next(value);
        }
      }
    }

    // Do the clock phase second
    for (auto &output : out_signals_clk) {
      for (auto &out : outputs) {
        if (output == out) {
          output->next(value);
        }
      }
    }
  }
};

#endif