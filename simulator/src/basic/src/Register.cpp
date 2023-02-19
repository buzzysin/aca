#include "Register.h"

#include <TkArch/Observers.h>

Register::Register(Clock *clock) : _clock(clock) {
  _enable = new BehaviourSubject<data_t>(0);
  _input  = new BehaviourSubject<data_t>(0);
  _reset  = new BehaviourSubject<data_t>(0);
  _output = new BehaviourSubject<data_t>(0);

  // Assign events based on clock
  _clock->risingEdge()->subscribe(new obs<State>([this](State) {
    // When reset is high, set output to 0
    if (_reset->getValue() == 1) {
      _output->next(0);
    }
    // When enable is high, set output to input
    else if (_enable->getValue() == 1) {
      _output->next(_input->getValue());
    }
  }));

  _clock->fallingEdge()->subscribe(new obs<State>([this](State) {
    // We're ignoring falling edges
  }));
}

Register::~Register() {
  delete _enable;
  delete _input;
  delete _reset;
  delete _output;
}

LiftedObservable<data_t> *Register::dataChange() {
  return _output->asObservable<data_t>();
}

void Register::dataStore(data_t value) { _input->next(value); }

data_t Register::dataLoad() { return _output->getValue(); }

void Register::connectData(LiftedObservable<data_t> *data) {}

void Register::connectEnable(LiftedObservable<data_t> *enable) {
  enable->subscribe(
      new obs<data_t>([this](data_t value) { _enable->next(value); }));
}

void Register::connectReset(LiftedObservable<data_t> *reset) {
  reset->subscribe(
      new obs<data_t>([this](data_t value) { _reset->next(value); }));
}

LiftedObservable<data_t> *Register::enableChange() {
  return _enable->asObservable<data_t>();
}

void Register::enable() { _enable->next(1); }

bool Register::enabled() { return _enable->getValue() == 1; }

void Register::disable() { _enable->next(0); }

bool Register::disabled() { return !enabled(); }

LiftedObservable<data_t> *Register::resetChange() {
  return _reset->asObservable<data_t>();
}

void Register::reset() { _reset->next(1); }

void Register::pass() { _reset->next(0); }
