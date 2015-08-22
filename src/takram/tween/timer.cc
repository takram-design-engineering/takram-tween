//
//  takram/tween/timer.cc
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

#include "takram/tween/timer.h"

#include <cassert>

#include "takram/tween/interval.h"
#include "takram/tween/timeline.h"

namespace takram {
namespace tween {

#pragma mark Controlling tween

template <class Interval>
void Timer<Interval>::start() {
  assert(timeline_);
  assert(adaptor_);
  if (!adaptor_->running()) {
    timeline_->add(adaptor_);
    adaptor_->start(timeline_->now());
  }
}

template <class Interval>
void Timer<Interval>::stop() {
  assert(timeline_);
  assert(adaptor_);
  if (adaptor_->running()) {
    adaptor_->stop();
    timeline_->remove(adaptor_);
  }
}

template class Timer<Time>;
template class Timer<Frame>;

}  // namespace tween
}  // namespace takram
