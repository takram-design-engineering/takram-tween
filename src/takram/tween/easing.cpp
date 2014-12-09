//
//  takram/tween/easing.cpp
//
//  takram design engineering Confidential
//
//  Copyright (C) 2014 Shota Matsuda
//
//  All information contained herein is, and remains the property of takram
//  design engineering and its suppliers, if any. The intellectual and
//  technical concepts contained herein are proprietary to takram design
//  engineering and its suppliers and may be covered by U.S. and Foreign
//  Patents, patents in process, and are protected by trade secret or copyright
//  law. Dissemination of this information or reproduction of this material is
//  strictly forbidden unless prior written permission is obtained from takram
//  design engineering.
//

// For contributors: Adhere to Google's C++ coding style, except in case where
// your find rational conventions to retain readability, consistency and
// maintenability otherwise.
// Run cpplint to check your style errors before making a pull request.

#include "takram/tween/easing.h"

#include <cmath>

namespace takram {

namespace detail {

template <typename T, T (*Function)(T)>
typename Easing<T, Function>::ValueType Easing<T, Function>::In(T t) {
  return Function(t);
}

template <typename T, T (*Function)(T)>
typename Easing<T, Function>::ValueType Easing<T, Function>::Out(T t) {
  return 1.0 - In(1.0 - t);
}

template <typename T, T (*Function)(T)>
typename Easing<T, Function>::ValueType Easing<T, Function>::InOut(T t) {
  if (t < 0.5) {
    return In(2.0 * t) * 0.5;
  } else {
    return 0.5 + Out(2.0 * t - 1.0) * 0.5;
  }
}

template <typename T> T ease_back(T t) {
  const T s = 1.70158;
  return t * t * ((s + 1.0) * t - s);
}

template <typename T> T ease_bounce(T t) {
  const T v = 1.0 - t;
  T c;
  T d;
  if (v < (1.0 / 2.75)) {
    c = v;
    d = 0;
  } else if (v < (2.0 / 2.75)) {
    c = v - 1.5 / 2.75;
    d = 0.75;
  } else if (v < (2.5 / 2.75)) {
    c = v - 2.25 / 2.75;
    d = 0.9375;
  } else {
    c = v - 2.625 / 2.75;
    d = 0.984375;
  }
  return 1.0 - (7.5625 * c * c + d);
}

template <typename T> T ease_circular(T t) {
  return 1.0 - std::sqrt(1.0 - t * t);
}

template <typename T> T ease_cubic(T t) {
  return t * t * t;
}

template <typename T> T ease_elastic(T t) {
  using takram::math::tau;
  const T v = t - 1.0;
  const T p = 0.3;
  return -std::pow(2.0, 10.0 * v) * std::sin((v - p * 0.25) * tau<T>() / p);
}

template <typename T> T ease_exponential(T t) {
  if (t == 0.0) {
    return 0.0;
  }
  return std::pow(2.0, 10.0 * (t - 1.0));
}

template <typename T> T ease_linear(T t) {
  return t;
}

template <typename T> T ease_quadratic(T t) {
  return t * t;
}

template <typename T> T ease_quartic(T t) {
  return t * t * t * t;
}

template <typename T> T ease_quintic(T t) {
  return t * t * t * t * t;
}

template <typename T> T ease_sinusoidal(T t) {
  return 1.0 - std::cos(t * takram::math::half_pi<T>());
}

template float ease_back(float t);
template float ease_bounce(float t);
template float ease_circular(float t);
template float ease_cubic(float t);
template float ease_elastic(float t);
template float ease_exponential(float t);
template float ease_linear(float t);
template float ease_quadratic(float t);
template float ease_quartic(float t);
template float ease_quintic(float t);
template float ease_sinusoidal(float t);

template double ease_back(double t);
template double ease_bounce(double t);
template double ease_circular(double t);
template double ease_cubic(double t);
template double ease_elastic(double t);
template double ease_exponential(double t);
template double ease_linear(double t);
template double ease_quadratic(double t);
template double ease_quartic(double t);
template double ease_quintic(double t);
template double ease_sinusoidal(double t);

template class Easing<float, ease_back>;
template class Easing<float, ease_bounce>;
template class Easing<float, ease_circular>;
template class Easing<float, ease_cubic>;
template class Easing<float, ease_elastic>;
template class Easing<float, ease_exponential>;
template class Easing<float, ease_linear>;
template class Easing<float, ease_quadratic>;
template class Easing<float, ease_quartic>;
template class Easing<float, ease_quintic>;
template class Easing<float, ease_sinusoidal>;

template class Easing<double, ease_back>;
template class Easing<double, ease_bounce>;
template class Easing<double, ease_circular>;
template class Easing<double, ease_cubic>;
template class Easing<double, ease_elastic>;
template class Easing<double, ease_exponential>;
template class Easing<double, ease_linear>;
template class Easing<double, ease_quadratic>;
template class Easing<double, ease_quartic>;
template class Easing<double, ease_quintic>;
template class Easing<double, ease_sinusoidal>;

}  // namespace detail

}  // namespace takram
