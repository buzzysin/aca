#ifndef EITHER_H
#define EITHER_H

#include <optional>

template <typename T, typename U> class Either {
protected:
  std::optional<T> _left;
  std::optional<U> _right;

protected:
  Either(const std::optional<T> left, const std::optional<U> right);

public:
  Either(const T &left);
  Either(const U &right);
  Either(const Either<T, U> &other);

  bool hasLeft() const;
  bool hasRight() const;

  T left() const;
  U right() const;

  static Either<T, U> Left(const T &left);
  static Either<T, U> Right(const U &right);

  Either<T, U> operator=(const Either<T, U> &other) {
    this->_left = other._left;
    this->_right = other._right;

    return other;
  }
};

#include "Either.tpp"

#endif