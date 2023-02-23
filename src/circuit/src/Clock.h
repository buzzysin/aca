#ifndef CLOCK_H
#define CLOCK_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

#include <thread>

// This class has a weird name because it's a reserved word in C++.
class clock_
{
protected:
  std::thread _thread;

  bool _running;
  bool _halted;

  size_t _cycles;

public:
  sp<service<isa::logic_t>> state;

public:
  clock_() {
    _running = false;
    _halted  = false;

    _cycles = 0;

    state = std::make_shared<service<isa::logic_t>>(0);
  }
  virtual ~clock_() { state->disconnect(); }

  void tick() { state->next(!state->value()); }

  void cycle() {
    tick();
    tick();

    _cycles++;
  }

  void start() {
    _running = true;
    _halted  = false;

    _thread = std::thread([this]() {
      while (_running) {
        if (!_halted) {
          // std::this_thread::sleep_for(std::chrono::milliseconds(1));
          info("Initiating new cycle...");
          cycle();
        }
      }
    });
  }

  void stop() {
    _running = false;
    _thread.join();
  }

  void halt() { _halted = true; }
};

#endif
