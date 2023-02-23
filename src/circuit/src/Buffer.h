#ifndef BUFFERED_H
#define BUFFERED_H

#include <TkArch/Signals.h>

#include "Connected.h"

template <typename T>
class buffer : public virtual connected
{
public:
  sp<service<T>> out_data{T()};
  sp<service<T>> in_oe_data{T()};
  sp<service<T>> in_data{T()};

  buffer() {
    watch<T>(this->in_oe_data, [this](const T &value) {
      if (value) {
        out_data->next(in_data->value());
      }
    });
  }
};

#endif