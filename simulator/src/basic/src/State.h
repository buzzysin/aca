#ifndef STATE_H
#define STATE_H

#include <ostream>

enum State { UNDEFINED, HIGH, LOW };

inline std::ostream &operator<<(std::ostream &os, State state) {
  switch (state) {
  case UNDEFINED:
    os << "UNDEFINED";
    break;
  case HIGH:
    os << "HIGH";
    break;
  case LOW:
    os << "LOW";
    break;
  }
  return os;
}

#endif // STATE_H