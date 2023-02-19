#ifndef TRISTATE_BUFFER_H
#define TRISTATE_BUFFER_H

#include <TkArch/Events.h>

#include "Data.h"

class TristateBuffer
{
protected:
  BehaviourSubject<data_t> *_input;
  BehaviourSubject<data_t> *_control;
  BehaviourSubject<data_t> *_output;

public:
  TristateBuffer();
  ~TristateBuffer();

public:
  void connectSet(LiftedObservable<data_t> *set);
  void connectReset(LiftedObservable<data_t> *reset);
  void connectData(LiftedObservable<data_t> *data);

  LiftedObservable<data_t> *outputChange() const;
};

#endif