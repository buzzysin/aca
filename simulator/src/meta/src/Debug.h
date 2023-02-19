#ifndef DEBUG_H
#define DEBUG_H

#include <type_traits>

#if defined(DEBUG)
#define DEBUG_IMPL(Class)
#else
#define DEBUG_IMPL(Class)
#endif

#ifndef __DEBUG_FN__

#if defined(__PRETTY_FUNCTION__)
#define __DEBUG_FN__ __PRETTY_FUNCTION__
#elif defined(__FUNCTION__)
#define __DEBUG_FN__ __FUNCTION__
#else
#define __DEBUG_FN__ __func__
#endif

#endif

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

class Debug {
  struct Stream {
    enum Level { None, Error, Warning, Info, Debug, Trace };

    std::ostream *_ptr;
    Stream(std::ostream *ptr = &std::cout) : _ptr(ptr) {}
    Stream(const Stream &stream) : _ptr(stream._ptr) {}
    Stream(Stream &&stream) : _ptr(stream._ptr) { stream._ptr = nullptr; }
    Stream(const Stream *stream) : _ptr(stream->_ptr) {}
    bool _spaces = true;
    bool _quotes = false;
    Level _level = Debug;
    std::string _file = "";
    int _line = 0;
    std::string _function = "";
    std::string _color = "";
    bool _lines = true;
    std::string _endl = "\n";
    bool _preamble = false;
    bool _on = true;

    static std::mutex _mutex;
  } *_stream;

public:
  Debug();
  Debug(std::ostream *stream);
  Debug(const Debug &debug);
  Debug(Debug &&debug);
  virtual ~Debug();

public:
  Debug &at(const std::string &file, int line, const std::string &function);
  Debug &debug();
  Debug &info();
  Debug &warning();
  Debug &error();
  Debug &trace();
  Debug &spaces();
  Debug &nospaces();
  Debug &maybeSpaces();
  Debug &quotes();
  Debug &noquotes();
  Debug &lines();
  Debug &nolines();
  Debug &endl();
  Debug &enable();
  Debug &disable();

private:
  Debug &preamble();
  Debug &finish();
  Debug &penDown();
  Debug &penUp();
  Debug &attemptSpaces();

public:
  Debug &operator<<(const char *string);
  Debug &operator<<(size_t value);
  Debug &operator<<(int value);
  Debug &operator<<(bool value);
  Debug &operator<<(unsigned int value);
  Debug &operator<<(const std::string &string);

  template <typename Object>
  friend std::enable_if_t<std::is_class_v<typename Object::Metadata>, Debug &>
  operator<<(Debug &debug, const Object &object);
};

#if defined(DEBUG)

#define tkDebug(...)                                                           \
  Debug(&std::cerr).at(__FILE__, __LINE__, __FUNCTION__).debug() __VA_ARGS__
#define tkInfo(...)                                                            \
  Debug(&std::cerr).at(__FILE__, __LINE__, __FUNCTION__).info() __VA_ARGS__
#define tkWarning(...)                                                         \
  Debug(&std::cerr).at(__FILE__, __LINE__, __FUNCTION__).warning() __VA_ARGS__
#define tkError(...)                                                           \
  Debug(&std::cerr).at(__FILE__, __LINE__, __FUNCTION__).error() __VA_ARGS__
#define tkTrace(...)                                                           \
  Debug(&std::cerr).at(__FILE__, __LINE__, __FUNCTION__).trace() __VA_ARGS__

#else

#define tkDebug(...) Debug().debug().disable() __VA_ARGS__
#define tkInfo(...) Debug().info().disable() __VA_ARGS__
#define tkWarning(...) Debug().warning().disable() __VA_ARGS__
#define tkError(...) Debug().error().disable() __VA_ARGS__
#define tkTrace(...) Debug().trace().disable() __VA_ARGS__

#endif // DEBUG

#include "Debug.tpp"

#endif // DEBUG_H