#include "Processor.h"

Processor::Processor() {
  _clock = new Clock();
  _controlUnit = new ControlUnit(_clock);
  _memoryAddressRegister = new MemoryAddressRegister(_clock);
  _memoryBufferRegister = new MemoryBufferRegister(_clock);
  _arithmeticLogicUnit = new ArithmeticLogicUnit(_clock);
}

Processor::~Processor() {
  stop();

  delete _clock;
  delete _controlUnit;
  delete _memoryAddressRegister;
  delete _memoryBufferRegister;
  delete _arithmeticLogicUnit;
}

void Processor::start() {
  _clock->start();
}

void Processor::stop() {
  _clock->stop();
}