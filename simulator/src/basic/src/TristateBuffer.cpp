#include "TristateBuffer.h"

TristateBuffer::TristateBuffer() {
  _input   = new BehaviourSubject<data_t>(0);
  _control = new BehaviourSubject<data_t>(0);
  _output  = new BehaviourSubject<data_t>(0);
}

TristateBuffer::~TristateBuffer() {
  delete _input;
  delete _control;
  delete _output;
}

void TristateBuffer::connectSet(LiftedObservable<data_t> *set) {}

void TristateBuffer::connectReset(LiftedObservable<data_t> *reset) {}

void TristateBuffer::connectData(LiftedObservable<data_t> *data) {}