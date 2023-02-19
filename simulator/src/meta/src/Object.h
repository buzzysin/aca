#ifndef OBJECT_H
#define OBJECT_H

#include <iomanip>
#include <sstream>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif
#include <cstdint>
#ifdef __cplusplus
}
#endif

#define OBJECT(...)                                                            \
  friend class Debug;                                                          \
                                                                               \
public:                                                                        \
  std::string toString() const {                                               \
    auto i = reinterpret_cast<std::uintptr_t>(this);                           \
    std::stringstream stream;                                                  \
    stream << "0x" << std::hex << i;                                           \
    std::string name = metadata.toString();                                    \
    return name + "@" + stream.str();                                          \
  };                                                                           \
                                                                               \
public:                                                                        \
  class Metadata : public Object {                                             \
                                                                               \
  public:                                                                      \
    Metadata() = default;                                                      \
    ~Metadata() = default;                                                     \
                                                                               \
  public:                                                                      \
    std::string toString() const { return #__VA_ARGS__; }                      \
  } metadata;                                                                  \
                                                                               \
private:

class Object {
public:
  Object() = default;
  ~Object() = default;

public:
  std::string toString() const { return "Object"; };
};

#endif // OBJECT_H