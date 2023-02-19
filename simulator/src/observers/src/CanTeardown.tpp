#ifndef CAN_TEARDOWN_TPP
#define CAN_TEARDOWN_TPP

inline std::function<void(void)> CanTeardown::toFunction(CanTeardown *ct) {
  tkDebug() << "CanTeardown::toFunction()";

  if (!ct || !ct->_teardown) {
    return []() {};
  }

  return ct->_teardown;
}

inline CanTeardown *
CanTeardown::fromFunction(const std::function<void(void)> &f) {
  tkDebug() << "CanTeardown::fromFunction()";
  return new CanTeardown(f);
}

#endif