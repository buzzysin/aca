#ifndef CONNECTED_H
#define CONNECTED_H

#include <TkArch/Debug.h>
#include <TkArch/ISA.h>
#include <TkArch/Signals.h>

class connected
{
protected:
  std::vector<sp<connection>> _connections;

protected:
  connected() {}

  template <typename T>
  void watch(sp<service<T>> &in, std::function<void(T)> callback) {
    _connections.push_back(in->connect(callback));
  }

  template <typename T>
  void watch_buffer(sp<service<T>> &in1,
                    sp<service<T>> &out,
                    sp<service<isa::logic_t>> &enable) {
    _connections.push_back(enable->connect([&](isa::logic_t value) {
      if (value) {
        out->next(in1->value());
      }
    }));
  }

  template <typename T>
  void redirect(sp<service<T>> &in, sp<service<T>> &out) {
    _connections.push_back(in->connect([&](T value) { out->next(value); }));
  }

  template <typename T>
  void redirect(sp<service<T>> &in,
                sp<service<T>> &out,
                const types::mono_mapping<T> &transform) {
    _connections.push_back(
        in->connect([&](T value) { out->next(transform(value)); }));
  }

  template <typename T>
  void _report(const sp<service<T>> &in,
               const std::string &name,
               const std::string &message = "",
               const char *file           = "",
               int line                   = 0,
               const char *function       = "") {
    DEBUG_MODE_ONLY(_connections.push_back(in->connect([=](T value) {
      std::string text = printer::souligne(name) + " - " + message + ": " +
                         std::to_string(value);
      info_raw(file, line, function, text);
    })));
  }

  template <typename T, typename predicate>
  void _report_if(const sp<service<T>> &when,
                  const std::string &name,
                  const std::string &message = "",
                  const predicate &only      = &connected::always,
                  const char *file           = "",
                  int line                   = 0,
                  const char *function       = "") {
    DEBUG_MODE_ONLY(_connections.push_back(when->connect([=](T value) {
      if (only(value)) {
        std::string text = printer::souligne(name) + " - " + message + ": " +
                           std::to_string(value);
        info_raw(file, line, function, text);
      }
    })));
  }

  static bool truthy(isa::logic_t value) { return value == 1; }
  static bool falsy(isa::logic_t value) { return value == 0; }
  static bool always(isa::logic_t) { return true; }

  virtual ~connected() { unwatch_all(); }

  void unwatch_all() {
    for (auto &connection : _connections) {
      connection->disconnect();
    }
  }
};

#define report(in, name, message)                                              \
  this->_report(in, name, message, __FILE__, __LINE__, PRETTY_FUNCTION)

#define report_if(when, name, message, only)                                   \
  this->_report_if(                                                            \
      when, name, message, only, __FILE__, __LINE__, PRETTY_FUNCTION)

#endif