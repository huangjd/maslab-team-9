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

template <typename T = double>
T randomValue(T upper = 1.0) {
  double i = (rand() % 32768) / 32768.0;
  return i * upper;
}

template <typename T>
T randomValue(T lower, T upper) {
  double i = (rand() % 32768) / 32768.0;
  return i * (upper - lower) + lower;
}

#endif //__COMMON_H__
