#ifndef OUTPUT_H
#define OUTPUT_H

template <typename T>
class output
{
public:
  service<T> out_data{T()};
};

#endif