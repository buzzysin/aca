#ifndef ERROR_CONTEXT_H
#define ERROR_CONTEXT_H

#include <functional>
#include <iostream>

#include "TkArch/Meta.h"

static void errorContext(const std::function<void()> &callback);

inline void errorContext(const std::function<void()> &callback) {
  tkDebug() << "static ::errorContext()";
  try {
    callback();
  } catch (std::exception &error) {
    std::cerr << "Error: " << error.what() << "\n";
  }
}

#endif