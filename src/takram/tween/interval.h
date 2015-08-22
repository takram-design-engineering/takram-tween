//
//  takram/tween/interval.h
//
//  The MIT License
//
//  Copyright (C) 2014-2015 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#pragma once
#ifndef TAKRAM_TWEEN_INTERVAL_H_
#define TAKRAM_TWEEN_INTERVAL_H_

#include <cassert>

#include "takram/tween/types.h"

namespace takram {
namespace tween {

template <class T>
class Interval final {
 public:
  using Value = T;

 public:
  Interval();
  Interval(T value);  // Intentionally implicit

  // Copy semantics
  Interval(const Interval&) = default;
  Interval& operator=(const Interval&) = default;

  // Comparison
  bool operator==(const Interval& other) const;
  bool operator!=(const Interval& other) const;
  bool operator<(const Interval& other) const;
  bool operator>(const Interval& other) const;
  bool operator<=(const Interval& other) const;
  bool operator>=(const Interval& other) const;

  // Arithmetic
  Interval& operator+=(const Interval& other);
  Interval& operator-=(const Interval& other);
  Interval operator+(const Interval& other) const;
  Interval operator-(const Interval& other) const;
  Unit operator/(const Interval& other) const;

  // Attributes
  bool empty() const { return !value_; }
  Value count() const { return value_; }

 private:
  T value_;
};

using Time = Interval<TimeValue>;
using Frame = Interval<FrameValue>;

#pragma mark -

template <class T>
inline Interval<T>::Interval() : value_() {}

template <class T>
inline Interval<T>::Interval(T value) : value_(value) {}

#pragma mark Arithmetic

template <class T>
inline Interval<T>& Interval<T>::operator+=(const Interval& other) {
  value_ += other.value_;
  return *this;
}

template <class T>
inline Interval<T>& Interval<T>::operator-=(const Interval& other) {
  value_ -= other.value_;
  return *this;
}

template <class T>
inline Interval<T> Interval<T>::operator+(const Interval& other) const {
  return Interval<T>(value_ + other.value_);
}

template <class T>
inline Interval<T> Interval<T>::operator-(const Interval& other) const {
  return Interval<T>(value_ - other.value_);
}

template <class T>
inline Unit Interval<T>::operator/(const Interval& other) const {
  return static_cast<Unit>(value_) / other.value_;
}

#pragma mark Comparison

template <class T>
inline bool Interval<T>::operator==(const Interval<T>& other) const {
  return value_ == other.value_;
}

template <class T>
inline bool Interval<T>::operator!=(const Interval<T>& other) const {
  return !operator==(other);
}

template <class T>
inline bool Interval<T>::operator<(const Interval& other) const {
  return value_ < other.value_;
}

template <class T>
inline bool Interval<T>::operator>(const Interval& other) const {
  return value_ > other.value_;
}

template <class T>
inline bool Interval<T>::operator<=(const Interval& other) const {
  return (value_ < other.value_ || value_ == other.value_);
}

template <class T>
inline bool Interval<T>::operator>=(const Interval& other) const {
  return (value_ > other.value_ || value_ == other.value_);
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_INTERVAL_H_
