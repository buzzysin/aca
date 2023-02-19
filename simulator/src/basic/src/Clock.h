#ifndef CLOCK_H
#define CLOCK_H

#include <thread>

#include "State.h"

#include <TkArch/Events.h>

class Clock
{
protected:
  // Inputs

  // Outputs
  BehaviourSubject<State> *_state;
  BehaviourSubject<State> *_risingEdge;
  BehaviourSubject<State> *_fallingEdge;

  // Internal
  std::thread _thread;
  bool _running;
  bool _paused;

public:
  Clock();
  virtual ~Clock();

public:
  void start();
  void pause();
  void stop();
  void reset();

  void tick();
  void cycle();

  CanSubscribe<State> *stateChange() const;

  CanSubscribe<State> *risingEdge() const;
  CanSubscribe<State> *fallingEdge() const;

  bool running() const;

protected:
  virtual void _run();
};

#endif // CLOCK_H
