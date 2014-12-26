//
//  takram/tween/timeline_host.h
//
//  MIT License
//
//  Copyright (C) 2014-2015 Shota Matsuda
//  Copyright (C) 2014-2015 takram design engineering
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

#include "takram/tween/tween.h"

namespace takram {
namespace tween {

template <typename Interval>
class Timeline;

template <typename Interval = Time>
class TimelineHost {
 public:
  using Timeline = Timeline<Interval>;

  // Constructors
  virtual ~TimelineHost() = 0;

  // Managing tweens
  template <typename... Args>
  Tween<Interval> tween(Args&&... args);

  // Accessing timeline
  virtual Timeline& timeline() = 0;
  virtual const Timeline& timeline() const = 0;
};

#pragma mark - Inline Implementations

template <typename Interval>
inline TimelineHost<Interval>::~TimelineHost() {}

#pragma mark Managing tweens

template <typename Interval>
template <typename... Args>
inline Tween<Interval> TimelineHost<Interval>::tween(Args&&... args) {
  auto tween = Tween<Interval>(args..., &timeline());
  tween.start();
  return tween;
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TIMELINE_HOST_H_
