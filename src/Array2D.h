#ifndef __ARRAY2D_H__
#define __ARRAY2D_H__

#include <vector>
#include "Common.h"

template <class T,
          class Alloc = std::allocator<T>>
class Array2D : public std::vector<T, Alloc> {
  using typename std::vector<T, Alloc>::size_type;
  using std::vector<T, Alloc>::resize;
  size_type w;
  size_type h;
public:
  Array2D() : w(0), h(0) {}

  Array2D(size_type width, size_type height) : w(width), h(height) {
    resize(w * h);
  }

  Array2D(const Array2D&) = default;

  T& operator()(size_type width, size_type height) {
    return this->operator [](width * w + height);
  }

  const T& operator ()(size_type width, size_type height) const {
    return this->operator [](width * w + height);
  }

  T& at(size_type width, size_type height) {
    if (width >= w || height >= h) {
      throw std::exception();
    }
    return this->operator [](width * w + height);
  }

  const T& at(size_type width, size_type height) const {
    if (width >= w || height >= h) {
      throw std::exception();
    }
    return this->operator [](width * w + height);
  }

  std::pair<size_type, size_type> size(int) const {
    return std::pair<size_type, size_type>(w, h);
  }

  void resize(size_type width, size_type height) {
    resize(width * height);
  }

//  resize_block(size_t w1, size_t w2, size_t w3, size_t w4)

};


#endif // __ARRAY2D_H__
