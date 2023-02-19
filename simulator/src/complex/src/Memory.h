#ifndef MEMORY_H
#define MEMORY_H

#include <TkArch/Basic.h>

class Memory
{
protected:
  int _size;


  std::vector<Register *> _registers;

public:
  Memory(Clock *clock, int size);
  virtual ~Memory();

public:
  void write(data_t address, data_t value);
  data_t read(data_t address);
};

inline Memory::Memory(Clock *clock, int size)
{
  _size = size;
  

  for (int i = 0; i < _size; i++)
  {
    _data[i] = 0;
  }
}


#endif