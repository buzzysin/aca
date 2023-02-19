#include "Clock.h"

#include <TkArch/Events.h>

Clock::Clock()
    : _state(new BehaviourSubject<State>(State::LOW)),
      _risingEdge(new BehaviourSubject<State>(HIGH)),
      _fallingEdge(new BehaviourSubject<State>(LOW)), _thread(),
      _running(false), _paused(false) {
  // An observer that will stop the clock
  // when it completes
  _state->subscribe(new Observer<State>([this]() { _running = false; }));

  // An observer that will emit a rising edge
  // when the clock goes from low to high
  _state->subscribe(new Observer<State>([this](State) {
    if (_state->value() == State::HIGH) {
      _risingEdge->next(HIGH);
    }
  }));

  // An observer that will emit a falling edge
  // when the clock goes from high to low
  _state->subscribe(new Observer<State>([this](State) {
    if (_state->value() == State::LOW) {
      _fallingEdge->next(LOW);
    }
  }));

  _risingEdge->subscribe(new Observer<State>([this]() {
    // Stderr debug
    std::cerr << "Clock: Rising edge" << std::endl;
  }));

  _fallingEdge->subscribe(new Observer<State>([this]() {
    // Stderr debug
    std::cerr << "Clock: Falling edge" << std::endl;
  }));
}

Clock::~Clock() {
  stop();
  delete _state;
}

void Clock::start() {
  if (!_running) {
    _running = true;

    // If paused was not set, then it was not called,
    // meaning this is a first time start
    if (!_paused) {
      _thread = std::thread(&Clock::_run, this);
    }
  }

  // We are no longer paused
  _paused = false;
}

void Clock::pause() {
  if (_running) {
    // We need to stop ticking, but not join the thread
    _running = false;
    _paused = true;
  }
}

void Clock::reset() {
  // "Asynchronously" reset the clock
  _state->next(State::LOW);
}

// Warning - if a long operation is running, this will block
// for a long time, until it completes. If the operation
// does not complete, this will block forever.
void Clock::stop() {
  if (_running) {
    // Stop ticking
    pause();
  }

  // Join the thread
  if (_thread.joinable()) {
    _thread.join();
  }
}

void Clock::tick() {
  if (_state->value() == UNDEFINED) {
    _state->next(State::LOW);
  } else if (_state->value() == State::LOW) {
    _state->next(State::HIGH);
  } else if (_state->value() == State::HIGH) {
    _state->next(State::LOW);
  } else { // In case someone set a weird value, which should never happen
    _state->next(UNDEFINED);
  }
}

// Tick once to change the state, then tick again to emit the edge
void Clock::cycle() {
  tick();
  tick();
}

bool Clock::running() const { return _running; }

CanSubscribe<State> *Clock::stateChange() const { return _state; }

CanSubscribe<State> *Clock::risingEdge() const { return _risingEdge; }

CanSubscribe<State> *Clock::fallingEdge() const { return _fallingEdge; }

void Clock::_run() {
  while (_running) {
    tick();
  }
}