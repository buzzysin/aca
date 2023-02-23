#ifndef EVENT_H
#define EVENT_H

#include "Types.h"

#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <variant>

#include <TkArch/Debug.h>

class undefined_callback : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

template <typename T>
class event
{

  template <typename U>
  friend class event_safe;

protected:
  types::next_fn<T> _inner_next;
  types::error_fn _inner_error;
  types::complete_fn _inner_complete;

public:
  event() {}
  event(types::next_fn<T> next) { _inner_next = next; }
  event(types::next_fn<T> next, types::error_fn error) {
    _inner_next  = next;
    _inner_error = error;
  }
  event(types::next_fn<T> next,
        types::error_fn error,
        types::complete_fn complete) {
    _inner_next     = next;
    _inner_error    = error;
    _inner_complete = complete;
  }
  event(types::error_fn error) { _inner_error = error; }
  event(types::error_fn error, types::complete_fn complete) {
    _inner_error    = error;
    _inner_complete = complete;
  }
  event(types::next_fn<T> next, types::complete_fn complete) {
    _inner_next     = next;
    _inner_complete = complete;
  }
  event(types::complete_fn complete) { _inner_complete = complete; }

  virtual ~event() {}

  virtual void next(const T &value) {
    if (_inner_next) {
      _inner_next(value);
    } else {
      throw undefined_callback("Undefined next callback.");
    }
  };
  virtual void error(const std::exception &e) {
    if (_inner_error) {
      _inner_error(e);
    } else {
      throw undefined_callback("Undefined error callback.");
    }
  };
  virtual void complete() {
    if (_inner_complete) {
      _inner_complete();
    } else {
      throw undefined_callback("Undefined complete callback.");
    }
  };
};

template <typename T>
class event_safe : public event<T>
{

protected:
  sp<event<T>> _inner;

public:
  event_safe(sp<event<T>> inner) {
    _inner                = inner;
    auto _safe_inner_next = ([&](const T &value) {
      if (_inner->_inner_next) {
        try {
          _inner->next(value);
        } catch (...) {
          // do nothing
        }
      } else {
        // do nothing
      }
    });

    auto _safe_inner_error = ([&](const std::exception &e) {
      if (_inner->_inner_error) {
        try {
          _inner->error(e);
        } catch (...) {
          // do nothing
        }
      } else {
        // do nothing
      }
    });

    auto _safe_inner_complete = ([&]() {
      if (_inner->_inner_complete) {
        try {
          _inner->complete();
        } catch (...) {
          // do nothing
        }
      } else {
        // do nothing
      }
    });

    event<T>::_inner_next     = _safe_inner_next;
    event<T>::_inner_error    = _safe_inner_error;
    event<T>::_inner_complete = _safe_inner_complete;
  }

  event_safe(types::next_fn<T> next)
      : event_safe(std::make_shared<event<T>>(next)) {}

  virtual ~event_safe() {
    // delete _inner;
    // _inner = nullptr;
  }
};

class disconnectable
{
public:
  virtual void disconnect() = 0;
};

class connection;

template <typename T>
class connectable
{
public:
  virtual sp<connection> connect(sp<event<T>> output) = 0;
};

class like_connection : public disconnectable
{
protected:
  bool _accepting;
};

class connection : public virtual like_connection,
                   public std::enable_shared_from_this<connection>
{
protected:
  types::dc_fn _disconnect;
  // parents
  std::vector<sp<connection>> _connections;
  // teardowns
  std::vector<sp<connection>> _dependents;
  std::vector<types::dc_fn> _dependent_fns;

public:
  connection() { _accepting = true; }
  connection(types::dc_fn unsubscribe) {
    _accepting  = true;
    _disconnect = unsubscribe;
  }

  virtual ~connection() {
    // Print #parents, #dependents, #dependent_fns
  }

  virtual void disconnect() {
    std::vector<std::exception> errors;

    if (_accepting) {
      _accepting = false;

      for (auto &conn : _connections) {
        try {
          conn->remove(shared_from_this());
        } catch (const std::exception &e) {
          errors.push_back(e);
        }
      }

      if (_disconnect) {
        try {
          _disconnect();
        } catch (const std::exception &e) {
          errors.push_back(e);
        }
      }

      for (auto &dependent : _dependents) {
        try {
          dependent->disconnect();
        } catch (const std::exception &e) {
          errors.push_back(e);
        }
      }

      for (auto &dependent_fn : _dependent_fns) {
        try {
          dependent_fn();
        } catch (const std::exception &e) {
          errors.push_back(e);
        }
      }

      if (errors.size() > 0) {
        throw std::runtime_error("Errors occurred while disconnecting.");
      }
    }
  };

  virtual void add(sp<connection> conn) {
    if (conn == shared_from_this()) {
      return;
    }

    if (!_accepting) {
      conn->disconnect();
      return;
    }

    if (!conn->_accepting && conn->_has_dependent(shared_from_this())) {
      return;
    }

    conn->_add_conn(shared_from_this());
  }

  virtual void add(types::dc_fn fn) {
    if (!_accepting) {
      fn();
      return;
    }

    _dependent_fns.push_back(fn);
  }

  virtual void add(const std::monostate &) {}

  virtual void remove(sp<connection> conn) {
    std::remove_if(_dependents.begin(),
                   _dependents.end(),
                   [&conn](sp<connection> c) { return c == conn; });

    conn->_remove_conn(shared_from_this());
  };

  virtual void remove(types::dc_fn fn) {
    std::remove_if(_dependent_fns.begin(),
                   _dependent_fns.end(),
                   [&fn](types::dc_fn &f) { return &f == &fn; });
  };

protected:
  virtual void _add_conn(sp<connection> conn) { _connections.push_back(conn); };

  virtual void _remove_conn(sp<connection> conn) {
    std::remove_if(_connections.begin(),
                   _connections.end(),
                   [&conn](sp<connection> c) { return c == conn; });

    // delete conn;
  };

  virtual bool _has_dependent(sp<connection> conn) {
    return std::find_if(_connections.begin(),
                        _connections.end(),
                        [&conn](sp<connection> c) { return c == conn; }) !=
           _connections.end();
  };
};

template <typename T>
class watcher : public connection, public virtual event<T>
{
protected:
  bool _watching                = true;
  sp<watcher<T>> _w_destination = nullptr;
  sp<event<T>> _o_destination   = nullptr;
  sp<event<T>> _s_destination   = nullptr;

public:
  watcher() { _s_destination = std::make_shared<event_safe<T>>(); }

  watcher(sp<watcher<T>> destination) {
    _w_destination = destination;

    _w_destination->add(this);
  }

  watcher(sp<event<T>> destination) { _o_destination = destination; }

  virtual ~watcher() {
    // delete _s_destination;
    // delete _o_destination;
    // delete _w_destination;
  }

  virtual void next(const T &value) {
    if (_watching) {

      _next(value);
    }
  }

  virtual void error(const std::exception &e) {
    if (_watching) {
      _watching = false;

      _error(e);
    }
  }

  virtual void complete() {
    if (_watching) {
      _watching = false;

      _complete();
    }
  }

  virtual void disconnect() {
    _watching = false;
    connection::disconnect();
  }

protected:
  virtual void _next(const T &value) {
    if (auto d = _w_destination) {
      d->next(value);
    } else if (auto d = _o_destination) {
      d->next(value);
    } else if (_s_destination) {
      _s_destination->next(value);
    } else {
      throw std::runtime_error("No destination for watcher.");
    }
  }

  virtual void _error(const std::exception &e) {
    if (auto d = _w_destination) {
      d->error(e);
    } else if (auto d = _o_destination) {
      // } else if (auto d = _o_destination) {
      d->error(e);
    } else {
      _s_destination->error(e);
    }
  }

  virtual void _complete() {
    if (auto d = _w_destination) {
      d->complete();
    } else if (auto d = _o_destination) {
      // } else if (auto d = _o_destination) {
      d->complete();
    } else {
      _s_destination->complete();
    }
  }
};

template <typename T>
class source : public connection, public virtual connectable<T>
{
protected:
  types::mapping<sp<watcher<T>>, sp<connection>> _inner_connect;
  types::mapping<sp<watcher<T>>, types::dc_fn> _inner_connect_fn;
  types::consumer<sp<watcher<T>>> _inner_consumer;

public:
  source() {}

  source(const types::mapping<sp<watcher<T>>, sp<connection>> &connect) {
    _inner_connect = connect;
  }

  source(const types::mapping<sp<watcher<T>>, types::dc_fn> connect_fn) {
    _inner_connect_fn = connect_fn;
  }

  source(const types::consumer<sp<watcher<T>>> connect_fn) {
    _inner_consumer = connect_fn;
  }

  virtual ~source() {}

  virtual sp<connection> connect(sp<event<T>> output) {
    sp<watcher<T>> output_watcher = std::make_shared<watcher<T>>(output);

    auto _connection = this->_try_connect(output_watcher);

    if (std::holds_alternative<sp<connection>>(_connection)) {
      auto c = std::get<sp<connection>>(_connection);

      c->add(output_watcher);

    } else if (std::holds_alternative<types::dc_fn>(_connection)) {
      auto fn = std::get<types::dc_fn>(_connection);

      output_watcher->add(fn);
    }

    return output_watcher;
  }

  virtual sp<connection> connect(const types::next_fn<T> &next) {
    auto _output = std::make_shared<event_safe<T>>(next);

    auto output_watcher = std::make_shared<watcher<T>>(_output);

    auto _connection = _try_connect(output_watcher);

    if (std::holds_alternative<sp<connection>>(_connection)) {
      auto c = std::get<sp<connection>>(_connection);

      c->add(output_watcher);
    } else if (std::holds_alternative<types::dc_fn>(_connection)) {
      auto fn = std::get<types::dc_fn>(_connection);

      output_watcher->add(fn);
    }

    return output_watcher;
  }

protected:
  virtual std::variant<sp<connection>, types::dc_fn, std::monostate>
  _try_connect(sp<watcher<T>> watch) {
    try {
      if (_inner_connect) {
        return this->_inner_connect(watch);
      } else if (_inner_connect_fn) {
        auto result = _inner_connect_fn(watch);
        return std::monostate();
      } else {
        _inner_consumer(watch);
        return std::monostate();
      }
    } catch (const std::exception &e) {
      if (auto w = watch) {
        w->error(e);
      }
    }
    return std::monostate();
  }
};

template <typename T>
class multi_source : public source<T>, public virtual like_connection
{
protected:
  std::vector<sp<event<T>>> _watchers;
  std::vector<sp<event<T>>> _current_watchers;
  bool _watching = true;

  bool _has_error = false;
  std::exception _thrown_error;
  types::mapping<sp<watcher<T>>, sp<connection>> _superclass_inner_connect;

public:
  multi_source() {
    _superclass_inner_connect = source<T>::_inner_connect;

    this->_inner_connect = ([&](sp<watcher<T>> watcher) -> sp<connection> {
      this->_throw_if_declined();
      this->_check_connection(watcher);
      return this->_connect_inner(watcher);
    });
  }

  virtual ~multi_source() { disconnect(); }

  void next(const T &value) {
    this->_throw_if_declined();
    if (!_watching)
      return;

    if (_current_watchers.empty()) {

      std::for_each(_watchers.begin(), _watchers.end(), [&](sp<event<T>> w) {
        _current_watchers.push_back(w);
      });
    }

    for (auto &w : _current_watchers) {
      w->next(value);
    }
  }

  void error(const std::exception &e) {
    this->_throw_if_declined();

    if (!_watching)
      return;

    _has_error    = true;
    _thrown_error = e;

    while (!_watchers.empty()) {
      auto w = _watchers.back();
      w->error(e);
      _watchers.pop_back();
    }
  }

  void complete() {
    this->_throw_if_declined();

    if (!_watching)
      return;

    while (!_watchers.empty()) {
      auto w = _watchers.back();
      w->complete();
      _watchers.pop_back();
    }
  }

  void disconnect() override {
    _watching  = false;
    _accepting = false;

    while (!_watchers.empty()) {
      auto w = _watchers.back();
      w->complete();
      _watchers.pop_back();
    }

    while (!_current_watchers.empty()) {
      auto w = _current_watchers.back();
      w->complete();
      _current_watchers.pop_back();
      // delete w;
    }
  }

protected:
  void _throw_if_declined() {
    if (!_accepting) {
      throw std::runtime_error(
          "connection is not accepting std::make_shared<clients");
    }
  }

  virtual void _check_connection(sp<event<T>> out) {
    if (auto _output = out) {

      if (_has_error) {
        _output->error(_thrown_error);
      } else if (!_watching) {
        _output->complete();
      }
    }
  }

  virtual sp<connection> _connect_inner(sp<event<T>> out) {
    if (_has_error || !_watching) {
      return std::make_shared<connection>([]() {});
    }
    _current_watchers.clear();
    _watchers.push_back(out);

    return std::make_shared<connection>([&] {
      _current_watchers.clear();
      std::remove_if(_watchers.begin(),
                     _watchers.end(),
                     [&](const sp<event<T>> w) { return w == out; });
    });
  }
};

template <typename T>
class service : public multi_source<T>
{
protected:
  T _value;

public:
  service(const T &value) { _value = value; }

  virtual ~service() {}

  void next(const T &value) {
    this->_value = value;
    multi_source<T>::next(value);
  }

  void error(const std::exception &e) { multi_source<T>::error(e); }

  void complete() { multi_source<T>::complete(); }

  void disconnect() override { multi_source<T>::disconnect(); }

  T value() {
    if (this->_has_error) {
      throw this->_thrown_error;
    }

    this->_throw_if_declined();

    return _value;
  }

protected:
  sp<connection> _connect_inner(sp<event<T>> out) override {
    sp<connection> c = multi_source<T>::_connect_inner(out);
    if (this->_watching) {
      out->next(this->_value);
    }
    return c;
  }

};

#endif