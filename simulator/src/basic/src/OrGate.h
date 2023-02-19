#ifndef OR_GATE_H
#define OR_GATE_H

#include <TkArch/Events.h>

#include "Data.h"

class OrGate
{

protected:
  BehaviourSubject<data_t> *_a;
  BehaviourSubject<data_t> *_b;

  BehaviourSubject<data_t> *_output;

public:
  OrGate();
  ~OrGate();

public:
  void setA(data_t a);
  void setB(data_t b);

  LiftedObservable<data_t> *outputChange() const;
};

#endif // OR_GATE_H