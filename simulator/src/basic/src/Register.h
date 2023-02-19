#ifndef REGISTER_H
#define REGISTER_H

#include "Clock.h"
#include "Data.h"

class Register
{
protected:
  // Inputs

  // Clock
  Clock *_clock;
  // Enable
  BehaviourSubject<data_t> *_enable;
  // Data
  BehaviourSubject<data_t> *_input;
  // Reset
  BehaviourSubject<data_t> *_reset;

  // Outputs

  // The value of the register
  BehaviourSubject<data_t> *_output;

public:
  Register(Clock *clock);

  virtual ~Register();

  // Subscribe to data changes
  virtual LiftedObservable<data_t> *dataChange();
  // The value of the register will be set on the next clock tick
  void dataStore(data_t value);
  // The value of the register will be returned immediately
  data_t dataLoad();

  // Connect an observable that passes data to the register
  void connectData(LiftedObservable<data_t> *data);

  // Connect an observable that passes enable to the register
  void connectEnable(LiftedObservable<data_t> *enable);

  // Connect an observable that passes reset to the register
  void connectReset(LiftedObservable<data_t> *reset);

  // Subscribe to enable changes
  virtual LiftedObservable<data_t> *enableChange();
  // The enable value will be set on the next clock tick
  void enable();
  // The check the current enable value
  bool enabled();
  // The enable value will be returned immediately
  void disable();
  // The check the current enable value
  bool disabled();

  // Subscribe to reset changes
  virtual LiftedObservable<data_t> *resetChange();
  // The reset value will be set on the next clock tick
  void reset();
  // The register will load the value on the next clock tick
  void pass();
};

#endif // REGISTER_H
