#ifndef __COMMON_H__
#define __COMMON_H__

template <typename T>
T sqr(const T& x) {
  return x * x;
}

template <typename T>
T clamp(const T& val, const T& lower, const T& upper) {
  if (val < lower) {
    return lower;
  } else if (val > upper) {
    return upper;
  } else {
    return val;
  }
}

#endif //__COMMON_H__
