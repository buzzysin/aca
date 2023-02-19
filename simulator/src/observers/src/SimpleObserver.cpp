#include "SimpleObserver.h"

#include <TkArch/Meta.h>

SimpleObserver::SimpleObserver(const std::optional<AnonNextFn> &next)
    : m_next(next) {
  tkDebug() << "SimpleObserver::SimpleObserver()";
}

SimpleObserver::SimpleObserver(const std::optional<ErrorFn> &next)
    : m_error(next) {
  tkDebug() << "SimpleObserver::SimpleObserver()";
}

SimpleObserver::SimpleObserver(const std::optional<CompleteFn> &next)
    : m_complete(next) {
  tkDebug() << "SimpleObserver::SimpleObserver()";
}

SimpleObserver::SimpleObserver(const std::optional<AnonNextFn> &next,
                               const std::optional<ErrorFn> &error)
    : m_next(next), m_error(error) {
  tkDebug() << "SimpleObserver::SimpleObserver()";
}

SimpleObserver::SimpleObserver(const std::optional<AnonNextFn> &next,
                               const std::optional<ErrorFn> &error,
                               const std::optional<CompleteFn> &complete)
    : m_next(next), m_error(error), m_complete(complete) {
  tkDebug() << "SimpleObserver::SimpleObserver()";
}

void SimpleObserver::next(void *value) {

  tkDebug() << "SimpleObserver::next()";

  if (m_next.has_value()) {
    m_next.value()(value);
  }
};

void SimpleObserver::error(std::exception &e) {
  tkDebug() << "SimpleObserver::error()";

  if (m_error.has_value()) {
    try {
      m_error.value()(e);
    } catch (const std::exception &error) {
      std::cerr << "Error: " << error.what() << "\n";
    }
  } else {
    std::cerr << "Error: " << e.what() << "\n";
  }
};

void SimpleObserver::complete() {

  tkDebug() << "SimpleObserver::complete()";

  if (m_complete.has_value()) {
    m_complete.value()();
  }
};