//
//  takram/tween/timer_adaptor.h
//
//  MIT License
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
#ifndef TAKRAM_TWEEN_TIMER_ADAPTOR_H_
#define TAKRAM_TWEEN_TIMER_ADAPTOR_H_

#include <cstddef>
#include <limits>
#include <random>

#include "takram/tween/adaptor.h"
#include "takram/tween/easing.h"
#include "takram/tween/types.h"

namespace takram {
namespace tween {

template <class Interval_>
class TimerAdaptor : public Adaptor<Interval_> {
 public:
  using Interval = Interval_;

 public:
  TimerAdaptor(const Interval& duration, const Callback& callback);

  // Disallow copy semantics
  TimerAdaptor(const TimerAdaptor&) = delete;
  TimerAdaptor& operator=(const TimerAdaptor&) = delete;

  // Move semantics
  TimerAdaptor(TimerAdaptor&&) = default;

  // Controlling the adaptor
  using Adaptor<Interval>::update;

  // Hash
  std::size_t object_hash() const override;
  std::size_t target_hash() const override;

 protected:
  // Updates against the local unit time
  void update(Unit unit) override {}
};

#pragma mark -

template <class Interval>
inline TimerAdaptor<Interval>::TimerAdaptor(
    const Interval& duration,
    const Callback& callback)
    : Adaptor<Interval>(LinearEasing::In, duration, Interval(), callback) {}

#pragma mark Hash

template <class Interval>
inline std::size_t TimerAdaptor<Interval>::object_hash() const {
  std::mt19937 generator((std::random_device()()));
  std::uniform_int_distribution<std::size_t> distribution(
      std::numeric_limits<std::size_t>::min(),
      std::numeric_limits<std::size_t>::max());
  return distribution(generator);
}

template <class Interval>
inline std::size_t TimerAdaptor<Interval>::target_hash() const {
  std::mt19937 generator((std::random_device()()));
  std::uniform_int_distribution<std::size_t> distribution(
      std::numeric_limits<std::size_t>::min(),
      std::numeric_limits<std::size_t>::max());
  return distribution(generator);
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TIMER_ADAPTOR_H_
