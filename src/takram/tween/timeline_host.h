//
//  takram/tween/timeline_host.h
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
#ifndef TAKRAM_TWEEN_TIMELINE_HOST_H_
#define TAKRAM_TWEEN_TIMELINE_HOST_H_

#include <utility>

#include "takram/tween/interval.h"
#include "takram/tween/timeline.h"
#include "takram/tween/timer.h"
#include "takram/tween/tween.h"

namespace takram {
namespace tween {

class TimelineHost {
 public:
  virtual ~TimelineHost() {}

  // Creating tweens
  template <class Interval, class... Args>
  Tween<Interval> tween(Args&&... args);
  template <class Interval, class... Args>
  Timer<Interval> timer(Args&&... args);

  // Accessing timeline
  template <class Interval>
  Timeline<Interval>& timeline();
  template <class Interval>
  const Timeline<Interval>& timeline() const;

 private:
  Timeline<Time> time_timeline_;
  Timeline<Frame> frame_timeline_;
};

#pragma mark -

#pragma mark Creating tweens

template <class Interval, class... Args>
inline tween::Tween<Interval> TimelineHost::tween(Args&&... args) {
  auto tween = tween::Tween<Interval>(
      std::forward<Args>(args)...,
      &timeline<Interval>());
  tween.start();
  return std::move(tween);
}

template <class Interval, class... Args>
inline tween::Timer<Interval> TimelineHost::timer(Args&&... args) {
  auto timer = tween::Timer<Interval>(
      std::forward<Args>(args)...,
      &timeline<Interval>());
  timer.start();
  return std::move(timer);
}

#pragma mark Accessing timeline

template <>
inline Timeline<Time>& TimelineHost::timeline() {
  return time_timeline_;
}

template <>
inline Timeline<Frame>& TimelineHost::timeline() {
  return frame_timeline_;
}

template <>
inline const Timeline<Time>& TimelineHost::timeline() const {
  return time_timeline_;
}

template <>
inline const Timeline<Frame>& TimelineHost::timeline() const {
  return frame_timeline_;
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TIMELINE_HOST_H_
