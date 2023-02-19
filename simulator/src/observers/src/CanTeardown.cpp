#include "CanTeardown.h"

CanTeardown::CanTeardown(const std::function<void(void)> &teardown)
    : _teardown(teardown){};

void CanTeardown::teardown() {

  tkDebug() << "CanTeardown::teardown()";

  _teardown();
};

const std::function<void(void)> *CanTeardown::reference() {

  tkDebug() << "CanTeardown::reference()";

  return &_teardown;
}
