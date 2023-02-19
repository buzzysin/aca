#include "OrGate.h"

OrGate::OrGate() {
  _a = new BehaviourSubject<data_t>(false);
  _b = new BehaviourSubject<data_t>(false);

  _output = new BehaviourSubject<data_t>(false);

  // When a changes, update the output
  _a->subscribe(
      new obs<data_t>([this](data_t a) { _output->next(a | _b->value()); }));

  // When b changes, update the output
  _b->subscribe(
      new obs<data_t>([this](data_t b) { _output->next(b | _a->value()); }));
}

OrGate::~OrGate() {
  delete _a;
  delete _b;

  delete _output;
}

void OrGate::setA(data_t a) { this->_a->next(a); }

void OrGate::setB(data_t b) { this->_b->next(b); }

LiftedObservable<data_t> *OrGate::outputChange() const {
  return this->_output->asObservable<data_t>();
}