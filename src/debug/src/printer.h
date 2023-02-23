#ifndef PRINTER_H
#define PRINTER_H

#include <cstdint>
#include <iostream>
#include <map>
#include <string>

#ifndef __has_feature
// GCC does not have __has_feature...
#  define __has_feature(feature) 0
#endif

#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#  ifdef __cplusplus
extern "C"
#  endif
    const char *
    __asan_default_options() {
  // Clang reports ODR Violation errors in mbedtls/library/certs.c.
  // NEED TO REPORT THIS ISSUE
  return "allocator_may_return_null=1";
}
#endif

#define IGNORE(x) (void)x

// try and use the compiler's builtin __PRETTY_FUNCTION__ macro
// if it's not available, one of the alternatives will be used
#if defined(__GNUC__) || defined(__clang__)
#  define PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#  define PRETTY_FUNCTION __FUNCSIG__
#elif defined(__FUNCSIG__)
#  define PRETTY_FUNCTION __FUNCSIG__
#else
#  define PRETTY_FUNCTION __func__
#endif

#define LEVEL_OFF -1
#define LEVEL_DEBUG 0
#define LEVEL_INFO 1
#define LEVEL_WARN 2
#define LEVEL_ERROR 3
#define LEVEL_FATAL 4

#if !defined(PRINTER)
#  define PRINTER_LEVEL LEVEL_OFF
#else
#  define PRINTER_LEVEL PRINTER
#endif

namespace printer {
  static std::size_t colour_index = 0;
  static std::map<std::string, std::size_t> colour_cache;

  static const std::string red     = "\033[0;31m";
  static const std::string green   = "\033[0;32m";
  static const std::string yellow  = "\033[0;33m";
  static const std::string blue    = "\033[0;34m";
  static const std::string magenta = "\033[0;35m";
  static const std::string cyan    = "\033[0;36m";
  static const std::string white   = "\033[0;37m";
  static const std::string reset   = "\033[0m";

  static const std::string bold      = "\033[1m";
  static const std::string muted     = "\033[2m";
  static const std::string italic    = "\033[3m";
  static const std::string underline = "\033[4m";
  static const std::string blink     = "\033[5m";
  static const std::string normal    = "\033[22m";

  static const std::string newline = "\n";

  static std::string colours[] = {
      red, green, yellow, blue, magenta, cyan, white};
  static const int num_colours = sizeof(colours) / sizeof(std::string);

  static inline std::string colour(std::string str) {
    return colours[colour_index % num_colours] + str + reset;
  }

  static inline std::string colour(std::string str, std::string colour) {
    return colour + str + reset;
  }

  static inline std::string colour(std::string str, int colour_index) {
    return colours[colour_index % num_colours] + str + reset;
  }

  static inline std::string embolden(std::string str) {
    return bold + str + reset;
  }

  static inline std::string italicize(std::string str) {
    return italic + str + reset;
  }

  static inline std::string souligne(std::string str) {
    return underline + str + reset;
  }

  static inline std::string blinken(std::string str) {
    return blink + str + reset;
  }

  static inline std::string mute(std::string str) {
    return muted + str + reset;
  }

  static inline std::string normalise(std::string str) {
    return normal + str + reset;
  }

#ifndef PRINTER_PADDING
#  define PRINTER_PADDING 25
#endif

  static size_t padding = PRINTER_PADDING;

  // if the string overflows the padding, it will be truncated with an ellipsis
  // appended
  static inline std::string align_left(std::string str) {
    if (str.length() > padding) {
      return str.substr(0, padding - 3) + "...";
    }

    return str + std::string(padding - str.length(), ' ');
  }

  // if the string overflows the padding, it will be truncated with an ellipsis
  // prepended
  static inline std::string align_right(std::string str) {
    if (str.length() > padding) {
      return "..." + str.substr(str.length() - padding + 3);
    }

    return std::string(padding - str.length(), ' ') + str;
  }

  static inline std::string align_center(std::string str) {
    if (str.length() > padding) {
      return "..." + str.substr(str.length() - padding + 3);
    }

    return std::string((padding - str.length()) / 2, ' ') + str +
           std::string((padding - str.length()) / 2, ' ');
  }

  // qDebug-like functions
  // format: <colour><bold>[ <header>  ]</bold></colour> :: <message>
  // where <header> is truncated __CALLLER__ @ __FILE__:__LINE__ if it overflows
  // the padding
  static inline std::string print(std::string _file,
                                  int _line,
                                  std::string _caller,
                                  std::string message) {
    // Fetch colour from cache
    std::string file = _file;
    std::string line = std::to_string(_line);

    if (colour_cache.find(_caller) == colour_cache.end()) {
      colour_cache[_caller] = colour_index++;
    }

    std::string file_name = file.substr(file.find_last_of("/") + 1);

    std::string header = _caller + " @ " + file_name + ":" + line;

    std::string colour = colours[colour_cache[_caller] % num_colours];

    std::string output = colour + bold + "[ " + align_right(header) + " ]" +
                         reset + " :: " + message + newline;

    return output;
  }

  static inline std::string _debug(std::string _file,
                                   int _line,
                                   std::string _caller,
                                   std::string message) {
    return muted + print(_file, _line, _caller, message);
  }

  static inline std::string _info(std::string _file,
                                  int _line,
                                  std::string _caller,
                                  std::string message) {
    return underline + print(_file, _line, _caller, message);
  }

} // namespace printer

#if (PRINTER)

#  if (PRINTER_LEVEL <= LEVEL_DEBUG)
#    define debug_raw(file, line, caller, message)                             \
      std::cerr << printer::_debug((file), (line), (caller), (message))
#    define debug(message)                                                     \
      debug_raw((__FILE__), (__LINE__), (PRETTY_FUNCTION), (message))
#  else
#    define debug(message)
#    define debug_raw(file, line, caller, message)
#  endif

#  if (PRINTER_LEVEL <= LEVEL_INFO)
#    define info_raw(file, line, caller, message)                              \
      std::cerr << printer::_info((file), (line), (caller), (message))
#    define info(message)                                                      \
      info_raw(__FILE__, __LINE__, PRETTY_FUNCTION, message)
#  else
#    define info_raw(file, line, caller, message)
#    define info(message)
#  endif
#endif

#if DEBUG_MODE
#  define DEBUG_MODE_ONLY(x) x
#else
#  define DEBUG_MODE_ONLY(x)
#endif

#endif