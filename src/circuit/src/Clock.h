#ifndef CLOCK_H
#define CLOCK_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>
#include <TkArch/Types.h>

#include <thread>

class clock_ : public subscription_manager
{
protected:
  isa::logic_t _state = isa::LOGIC_LOW;
  std::thread _thread;
  bool _running  = false;
  bool _halted   = false;
  size_t _cycles = 0;

public:
  sp<signal<isa::logic_t>> state;

public:
  clock_() { state = new_sp<signal<isa::logic_t>>(0); }

  void run() {
    while (_running) {
      if (_halted) {
        _state = isa::LOGIC_LOW;
      } else {
        cycle();
      }
    }
  }

  void start() {
    create_printer("Clock");
    print_info("Starting clock");
    _running = true;
    _thread  = std::thread(&clock_::run, this);
  }

  void stop() {
    create_printer("Clock");
    print_info("Stopping clock");

    _running = false;

    if (_thread.joinable())
      _thread.join();
  }

  void halt() { _halted = true; }

  void tick() {
    _state = _state == isa::LOGIC_LOW ? isa::LOGIC_HIGH : isa::LOGIC_LOW;
    state->next(_state);
  }

  void cycle() {
    create_printer("Clock");

    tick();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    tick();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    _cycles++;

    print_info("Ticking clock: " + std::to_string(_cycles) +
               " cycles, state = " + std::to_string(_state));
  }

  ~clock_() {}
};

#endif