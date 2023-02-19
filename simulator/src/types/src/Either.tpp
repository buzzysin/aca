#ifndef EITHER_TPP
#define EITHER_TPP

#include <stdexcept>

#include "Either.h"

template <typename T, typename U>
Either<T, U> Either<T, U>::Left(const T &left) {
  return Either<T, U>(left, std::nullopt);
}

template <typename T, typename U>
Either<T, U> Either<T, U>::Right(const U &right) {
  return Either<T, U>(std::nullopt, right);
}

template <typename T, typename U>
Either<T, U>::Either(const std::optional<T> left,
                     const std::optional<U> right) {
  if (left.has_value() == right.has_value())
    throw std::invalid_argument(
        "Either can't have both left and right or neither");
  else {
    this->_left = left;
    this->_right = right;
  }
}

template <typename T, typename U>
Either<T, U>::Either(const T &left)
    : Either(std::optional<T>(left), std::nullopt) {}

template <typename T, typename U>
Either<T, U>::Either(const U &right)
    : Either(std::nullopt, std::optional<U>(right)) {}

template <typename T, typename U>
Either<T, U>::Either(const Either<T, U> &other)
    : _left(other._left), _right(other._right) {}

template <typename T, typename U> T Either<T, U>::left() const {
  if (this->hasLeft())
    return this->_left.value();
  else
    throw std::invalid_argument("Either doesn't have left");
}

template <typename T, typename U> U Either<T, U>::right() const {
  if (this->hasRight())
    return this->_right.value();
  else
    throw std::invalid_argument("Either doesn't have right");
}

template <typename T, typename U> bool Either<T, U>::hasLeft() const {
  return this->_left.has_value();
}

template <typename T, typename U> bool Either<T, U>::hasRight() const {
  return this->_right.has_value();
}

#endif // EITHER_TPP