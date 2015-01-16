//
//  takram/tween/interval.h
//
//  MIT License
//
//  Copyright (C) 2014 Shota Matsuda
//  Copyright (C) 2014 takram design engineering
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

template <typename T>
class Interval final {
 public:
  using Value = T;

  // Constructors
  Interval();
  explicit Interval(T value);
  Interval(const Interval& other) = default;

  // Assignment
  Interval& operator=(const Interval& other) = default;

  // Comparison
  bool operator==(const Interval& other) const;
  bool operator!=(const Interval& other) const { return !operator==(other); }
  bool operator<(const Interval& other) const;
  bool operator>(const Interval& other) const;
  bool operator<=(const Interval& other) const;
  bool operator>=(const Interval& other) const;

  // Arithmetic
  Interval& operator+=(const Interval& other);
  Interval& operator-=(const Interval& other);

  // Properties
  Value count() const { return value_; }
  bool empty() const { return !value_; }

 private:
  // Data members
  T value_;
};

using Time = Interval<TimeValue>;
using Frame = Interval<FrameValue>;

#pragma mark - Inline Implementations

template <typename T>
inline Interval<T>::Interval()
    : value_(T()) {}

template <typename T>
inline Interval<T>::Interval(T value)
    : value_(value) {}

#pragma mark Arithmetic operators

template <typename T>
inline Interval<T> operator+(const Interval<T>& a, const Interval<T>& b) {
  return Interval<T>(a.count() + b.count());
}

template <typename T>
inline Interval<T> operator-(const Interval<T>& a, const Interval<T>& b) {
  return Interval<T>(a.count() - b.count());
}

template <typename T>
inline double operator/(const Interval<T>& a, const Interval<T>& b) {
  assert(b.count());
  return static_cast<double>(a.count()) / static_cast<double>(b.count());
}

template <typename T>
inline Interval<T>& Interval<T>::operator+=(const Interval& other) {
  value_ += other.value_;
  return *this;
}

template <typename T>
inline Interval<T>& Interval<T>::operator-=(const Interval& other) {
  value_ -= other.value_;
  return *this;
}

#pragma mark Comparison

template <typename T>
inline bool Interval<T>::operator==(const Interval<T>& other) const {
  return value_ == other.value_;
}

template <typename T>
inline bool Interval<T>::operator<(const Interval& other) const {
  return value_ < other.value_;
}

template <typename T>
inline bool Interval<T>::operator>(const Interval& other) const {
  return value_ > other.value_;
}

template <typename T>
inline bool Interval<T>::operator<=(const Interval& other) const {
  return (value_ < other.value_ || value_ == other.value_);
}

template <typename T>
inline bool Interval<T>::operator>=(const Interval& other) const {
  return (value_ > other.value_ || value_ == other.value_);
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_INTERVAL_H_
