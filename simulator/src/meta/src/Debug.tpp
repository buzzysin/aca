#ifndef DEBUG_TPP
#define DEBUG_TPP

#include "Debug.h"

template <typename Object>
std::enable_if_t<std::is_class_v<typename Object::Metadata>, Debug &>
operator<<(Debug &debug, const Object &object) {
  if (!debug._stream->_on) {
    return debug;
  }

  debug.nospaces();

  if (debug._stream) {
    auto stream = debug._stream;

    debug.attemptSpaces();

    debug.preamble();

    stream->_ptr->write(object.toString().c_str(), object.toString().length());

    debug.finish();
  }

  return debug.maybeSpaces();
}

#endif