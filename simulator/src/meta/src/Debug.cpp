#include "Debug.h"
#include "Macros.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <cstring>
#ifdef __cplusplus
}
#endif

#include <thread>

std::mutex Debug::Stream::_mutex = std::mutex();

Debug::Debug() : _stream(new Stream()) {}
Debug::Debug(std::ostream *stream) : _stream(new Stream(stream)) {}
Debug::Debug(const Debug &debug) : _stream(new Stream(debug._stream)) {}
Debug::Debug(Debug &&debug) : _stream(debug._stream) {
  debug._stream = nullptr;
}
Debug::~Debug() {
  if (_stream && _stream->_on) {
    delete _stream;
  }
}

Debug &Debug::at(const std::string &file, int line,
                 const std::string &function) {
  if (_stream && _stream->_on) {
    _stream->_file = file;
    _stream->_line = line;
    _stream->_function = function;
  }
  return *this;
}

Debug &Debug::debug() {
  if (_stream && _stream->_on) {

    _stream->_level = Stream::Debug;
    _stream->_color = "\033[0;36m"; // Cyan
  }
  return *this;
}

Debug &Debug::info() {
  if (_stream && _stream->_on) {
    _stream->_level = Stream::Info;
    _stream->_color = "\033[0;32m"; // Green
  }
  return *this;
}

Debug &Debug::warning() {
  if (_stream && _stream->_on) {
    _stream->_level = Stream::Warning;
    _stream->_color = "\033[0;33m"; // Yellow
  }
  return *this;
}

Debug &Debug::error() {
  if (_stream && _stream->_on) {
    _stream->_level = Stream::Error;
    _stream->_color = "\033[0;31m"; // Red
  }
  return *this;
}

Debug &Debug::trace() {
  if (_stream && _stream->_on) {
    _stream->_level = Stream::Trace;
    _stream->_color = "\033[0;35m"; // Magenta
  }
  return *this;
}

Debug &Debug::spaces() {
  if (_stream && _stream->_on) {
    _stream->_spaces = true;
  }
  return *this;
}

Debug &Debug::nospaces() {
  if (_stream && _stream->_on) {
    _stream->_spaces = false;
  }
  return *this;
}

Debug &Debug::maybeSpaces() {
  if (_stream && _stream->_on) {
    if (_stream->_spaces) {
      _stream->_ptr->put(' ');
    }
  }
  return *this;
}

Debug &Debug::quotes() {
  if (_stream && _stream->_on) {
    _stream->_quotes = true;
  }
  return *this;
}

Debug &Debug::noquotes() {
  if (_stream && _stream->_on) {
    _stream->_quotes = false;
  }
  return *this;
}

Debug &Debug::lines() {
  if (_stream && _stream->_on) {
    _stream->_lines = true;
  }
  return *this;
}

Debug &Debug::nolines() {
  if (_stream && _stream->_on) {
    _stream->_lines = false;
  }
  return *this;
}

Debug &Debug::endl() {
  if (_stream && _stream->_on) {
    _stream->_ptr->write(_stream->_endl.c_str(), _stream->_endl.length());
  }

  return *this;
}

Debug &Debug::enable() {
  if (_stream) {
    _stream->_on = true;
  }
  return *this;
}

Debug &Debug::disable() {
  if (_stream) {
    _stream->_on = false;
  }
  return *this;
}

Debug &Debug::preamble() {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    auto stream = _stream;

    if (!stream->_preamble) {
      penDown();

      if (stream->_file.length() > 0) {
        stream->_ptr->write(stream->_file.c_str(), stream->_file.length());
      }
      if (stream->_line > 0) {
        stream->_ptr->put(':');
        stream->_ptr->write(std::to_string(stream->_line).c_str(),
                            std::to_string(stream->_line).length());
      }

      if (stream->_function.length() > 0) {
        stream->_ptr->put(':');
        stream->_ptr->write(stream->_function.c_str(),
                            stream->_function.length());
      } else {
        stream->_ptr->put(':');
        stream->_ptr->write("anonymous", 9);
      }

      stream->_ptr->put(':');

      penUp();

      stream->_ptr->put('\n');

      stream->_preamble = true;
    }
  }

  return *this;
}

Debug &Debug::finish() {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    auto stream = _stream;

    if (stream->_lines) {
      stream->_ptr->write(stream->_endl.c_str(), stream->_endl.length());

      stream->_preamble = false;
    }
  }
  return *this;
}

Debug &Debug::penDown() {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    auto stream = _stream;

    if (stream->_color.length() > 0) {
      stream->_ptr->write(stream->_color.c_str(), stream->_color.length());
    }
  }
  return *this;
}

Debug &Debug::penUp() {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    auto stream = _stream;

    if (stream->_color.length() > 0) {
      stream->_ptr->write("\033[0m", 4);
    }
  }
  return *this;
}

Debug &Debug::attemptSpaces() {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    auto stream = _stream;

    if (stream->_spaces) {
      stream->_ptr->put(' ');
    }
  }
  return *this;
}

Debug &Debug::operator<<(int value) {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    nospaces();

    attemptSpaces();

    preamble();

    _stream->_ptr->write(std::to_string(value).c_str(),
                         std::to_string(value).length());

    finish();
  }

  return this->maybeSpaces();
}

Debug &Debug::operator<<(unsigned int value) {
  if (_stream && _stream->_on) {
    nospaces();

    attemptSpaces();

    preamble();

    _stream->_ptr->write(std::to_string(value).c_str(),
                         std::to_string(value).length());

    finish();
  }

  return this->maybeSpaces();
}

Debug &Debug::operator<<(size_t value) {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    nospaces();

    attemptSpaces();

    preamble();

    _stream->_ptr->write(std::to_string(value).c_str(),
                         std::to_string(value).length());

    finish();
  }

  return this->maybeSpaces();
}

Debug &Debug::operator<<(const char *string) {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    nospaces();

    attemptSpaces();

    preamble();

    _stream->_ptr->write(string, strlen(string));

    finish();
  }

  return this->maybeSpaces();
}

Debug &Debug::operator<<(bool value) {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    nospaces();

    preamble();

    std::string str = value ? "true" : "false";

    _stream->_ptr->write(str.c_str(), str.length());

    finish();
  }

  return this->maybeSpaces();
}

Debug &Debug::operator<<(const std::string &string) {
  if (_stream && _stream->_on) {
    // std::lock_guard<std::mutex> lock(_stream->_mutex);

    nospaces();

    attemptSpaces();

    preamble();

    _stream->_ptr->write(string.c_str(), string.length());

    finish();
  }

  return this->maybeSpaces();
}
