//
//  takram/tween/tween.cc
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

#include "takram/tween/tween.h"

#include <functional>

#include "glog/logging.h"

#include "takram/tween/interval.h"

namespace takram {
namespace tween {

#pragma mark Controlling tween

template <typename Interval>
void Tween<Interval>::start() {
  if (!timeline_) {
    LOG(WARNING) << "Attempt to control tween before setting timeline.";
  } else if (!adaptor_) {
    LOG(WARNING) << "Attempt to control tween without adaptor.";
  } else if (!adaptor_->running()) {
    timeline_->add(adaptor_);
    adaptor_->start(timeline_->now());
  }
}

template <typename Interval>
void Tween<Interval>::stop() {
  if (!timeline_) {
    LOG(WARNING) << "Attempt to control tween before setting timeline.";
  } else if (!adaptor_) {
    LOG(WARNING) << "Attempt to control tween without adaptor.";
  } else if (adaptor_->running()) {
    adaptor_->stop();
    timeline_->remove(adaptor_);
  }
}

#pragma mark Parameters

template <typename Interval>
const Easing& Tween<Interval>::easing() const {
  if (adaptor_) {
    return adaptor_->easing();
  } else {
    LOG(WARNING) << "Attempt to access parameter without adaptor.";
  }
  return null_easing_;
}

template <typename Interval>
void Tween<Interval>::set_easing(const Easing& value) {
  if (adaptor_) {
    return adaptor_->set_easing(value);
  } else {
    LOG(WARNING) << "Attempt to change parameter without adaptor.";
  }
}

template <typename Interval>
typename Interval::Value Tween<Interval>::duration() const {
  if (adaptor_) {
    return adaptor_->duration().count();
  } else {
    LOG(WARNING) << "Attempt to access parameter without adaptor.";
  }
  return Interval().count();
}

template <typename Interval>
void Tween<Interval>::set_duration(IntervalValue value) {
  if (adaptor_) {
    return adaptor_->set_duration(Interval(value));
  } else {
    LOG(WARNING) << "Attempt to change parameter without adaptor.";
  }
}

template <typename Interval>
typename Interval::Value Tween<Interval>::delay() const {
  if (adaptor_) {
    return adaptor_->delay().count();
  } else {
    LOG(WARNING) << "Attempt to access parameter without adaptor.";
  }
  return Interval().count();
}

template <typename Interval>
void Tween<Interval>::set_delay(IntervalValue value) {
  if (adaptor_) {
    return adaptor_->set_delay(Interval(value));
  } else {
    LOG(WARNING) << "Attempt to change parameter without adaptor.";
  }
}

template <typename Interval>
std::function<void()> Tween<Interval>::callback() const {
  if (adaptor_) {
    return adaptor_->callback();
  } else {
    LOG(WARNING) << "Attempt to access parameter without adaptor.";
  }
  return std::function<void()>();
}

template <typename Interval>
void Tween<Interval>::set_callback(const std::function<void()>& value) {
  if (adaptor_) {
    return adaptor_->set_callback(value);
  } else {
    LOG(WARNING) << "Attempt to change parameter without adaptor.";
  }
}

#pragma mark Timeline

template <typename Interval>
void Tween<Interval>::set_timeline(Timeline *value) {
  if (!adaptor_ && adaptor_->finished()) {
    timeline_ = value;
  } else {
    LOG(WARNING) << "Attempt to change timeline while tween is not finished.";
  }
}

template class Tween<Time>;
template class Tween<Frame>;

}  // namespace tween
}  // namespace takram
