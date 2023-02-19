#ifndef TYPES_H
#define TYPES_H

#include <functional>

template <typename T, typename R>
using UnaryFunction = std::function<R(T)>;


#endif