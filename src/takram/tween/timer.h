//
//  takram/tween/timer.h
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
#ifndef TAKRAM_TWEEN_TIMER_H_
#define TAKRAM_TWEEN_TIMER_H_

#include <cassert>
#include <memory>

#include "takram/tween/adaptor.h"
#include "takram/tween/interval.h"
#include "takram/tween/timer_adaptor.h"
#include "takram/tween/types.h"

namespace takram {
namespace tween {

template <class Interval>
class Timeline;

template <class Interval_ = Time>
class Timer final {
 public:
  using Interval = Interval_;
  using Timeline = Timeline<Interval>;

 private:
  using Adaptor = std::shared_ptr<Adaptor<Interval>>;

 public:
  Timer();
  explicit Timer(const Adaptor& adaptor, Timeline *timeline = nullptr);

  // Construct with duration
  explicit Timer(const Interval& duration, Timeline *timeline = nullptr);
  Timer(const Interval& duration,
        const Callback& callback,
        Timeline *timeline = nullptr);

  // Copy semantics
  Timer(const Timer&) = default;
  Timer& operator=(const Timer&) = default;

  // Comparison
  bool operator==(const Timer& other) const;
  bool operator!=(const Timer& other) const;

  // Controlling timer
  void start();
  void stop();

  // Attributes
  bool running() const;
  bool finished() const;
  bool empty() const;

  // Parameters
  const Interval& duration() const;
  void set_duration(const Interval& value);
  const Callback& callback() const;
  void set_callback(const Callback& value);

  // Timeline
  Timeline * timeline() const;
  void set_timeline(Timeline *value);

  // Adaptor
  Adaptor adaptor() const { return adaptor_; }

  // Conversion
  operator bool() const { return adaptor_ && timeline_; }

 private:
  Adaptor adaptor_;
  Timeline *timeline_;
};

#pragma mark -

template <class Interval>
inline Timer<Interval>::Timer()
    : timeline_(nullptr) {}

template <class Interval>
inline Timer<Interval>::Timer(const Adaptor& adaptor, Timeline *timeline)
    : adaptor_(adaptor),
      timeline_(timeline) {}

#pragma mark Construct with duration

template <class Interval>
inline Timer<Interval>::Timer(const Interval& duration, Timeline *timeline)
    : Timer(duration, nullptr, timeline) {}

template <class Interval>
inline Timer<Interval>::Timer(const Interval& duration,
                              const Callback& callback,
                              Timeline *timeline)
    : adaptor_(std::make_shared<TimerAdaptor<Interval>>(duration, callback)),
      timeline_(timeline) {}

#pragma mark Comparison

template <class Interval>
inline bool Timer<Interval>::operator==(const Timer& other) const {
  return adaptor_ == other.adaptor_ && timeline_ == other.timeline_;
}

template <class Interval>
inline bool Timer<Interval>::operator!=(const Timer& other) const {
  return !operator==(other);
}

#pragma mark Attributes

template <class Interval>
inline bool Timer<Interval>::running() const {
  return adaptor_ && adaptor_->running();
}

template <class Interval>
inline bool Timer<Interval>::finished() const {
  return adaptor_ && adaptor_->finished();
}

template <class Interval>
inline bool Timer<Interval>::empty() const {
  return !adaptor_;
}

#pragma mark Parameters

template <class Interval>
inline const Interval& Timer<Interval>::duration() const {
  assert(adaptor_);
  return adaptor_->duration();
}

template <class Interval>
inline void Timer<Interval>::set_duration(const Interval& value) {
  assert(adaptor_);
  return adaptor_->set_duration(value);
}

template <class Interval>
inline const Callback& Timer<Interval>::callback() const {
  assert(adaptor_);
  return adaptor_->callback();
}

template <class Interval>
inline void Timer<Interval>::set_callback(const Callback& value) {
  return adaptor_->set_callback(value);
}

#pragma mark Timeline

template <class Interval>
inline Timeline<Interval> * Timer<Interval>::timeline() const {
  return timeline_;
}

template <class Interval>
inline void Timer<Interval>::set_timeline(Timeline *value) {
  timeline_ = value;
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TIMER_H_
