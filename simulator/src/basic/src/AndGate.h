#ifndef AND_GATE_H
#define AND_GATE_H

#include <TkArch/Events.h>

#include "Data.h"

class AndGate
{

protected:
  BehaviourSubject<data_t> *_a;
  BehaviourSubject<data_t> *_b;

  BehaviourSubject<data_t> *_output;

public:
  AndGate();
  ~AndGate();

public:
  void setA(data_t a);
  void setB(data_t b);

  LiftedObservable<data_t> *outputChange() const;
};

#endif // AND_GATE_H