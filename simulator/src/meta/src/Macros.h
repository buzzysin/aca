#ifndef MACROS_H
#define MACROS_H

#define UNUSED(x) (void)(x)

// removes element at index `index` from vector `vec`
// resizing the vector so that the new size is `vec.size() - 1`
#define vecRemove(vec, index) vec.erase(vec.begin() + index)

#define vecRemoveItem(vec, item)                                               \
  {                                                                            \
    int __i = -1;                                                              \
    for (int __j = 0; __j < vec.size(); __j++) {                               \
      if (vec[__j] == item) {                                                  \
        __i = __j;                                                             \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (__i != -1) {                                                           \
      vecRemove(vec, __i);                                                     \
    }                                                                          \
  }

#endif