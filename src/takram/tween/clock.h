//
//  takram/tween/clock.h
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
#ifndef TAKRAM_TWEEN_CLOCK_H_
#define TAKRAM_TWEEN_CLOCK_H_

#include <chrono>

#include "takram/tween/interval.h"

namespace takram {
namespace tween {

template <class Interval_>
class Clock final {
 public:
  using Interval = Interval_;

 public:
  Clock();

  // Copy semantics
  Clock(const Clock&) = default;
  Clock& operator=(const Clock&) = default;

  // Comparison
  bool operator==(const Clock& other) const;
  bool operator!=(const Clock& other) const;

  // Controlling clock
  Interval advance();
  Interval now() const { return Interval(now_); }

 private:
  typename Interval::Value now_;
  typename Interval::Value birth_;
};

#pragma mark -

template <class Interval>
inline Clock<Interval>::Clock() : now_(), birth_(advance().count()) {}

#pragma mark Comparison

template <class Interval>
inline bool Clock<Interval>::operator==(const Clock& other) const {
  return (now_ == other.now_ && birth_ == other.birth_);
}

template <class Interval>
inline bool Clock<Interval>::operator!=(const Clock& other) const {
  return !operator==(other);
}

#pragma mark Controlling clock

template <>
inline Time Clock<Time>::advance() {
  using Clock = std::chrono::high_resolution_clock;
  using Duration = std::chrono::microseconds;
  now_ = static_cast<TimeValue>(std::chrono::duration_cast<Duration>(
      Clock::now().time_since_epoch()).count()) / Duration::period::den;
  return Time(now_);
}

template <>
inline Frame Clock<Frame>::advance() {
  return Frame(++now_);
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_CLOCK_H_
