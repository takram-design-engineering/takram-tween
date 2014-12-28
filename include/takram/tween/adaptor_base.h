//
//  takram/tween/adaptor_base.h
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
#ifndef TAKRAM_TWEEN_ADAPTOR_BASE_H_
#define TAKRAM_TWEEN_ADAPTOR_BASE_H_

#include <iostream>
#include <cstddef>
#include <functional>

#include "takram/easing.h"

namespace takram {
namespace tween {

template <typename Interval_>
class AdaptorBase {
 public:
  using Interval = Interval_;

  // Constructors
  virtual ~AdaptorBase() = 0;

  // Disallow copy and assign
  AdaptorBase(const AdaptorBase&) = delete;
  AdaptorBase& operator=(const AdaptorBase&) = delete;

  // Controlling the adaptor
  void start(const Interval& now);
  void stop();
  void update(const Interval& now, bool callback = true);

  // Properties
  bool running() const { return running_; }
  bool finished() const { return finished_; }

  // Hash
  virtual std::size_t object_hash() const = 0;
  virtual std::size_t target_hash() const = 0;

  // Parameters
  const Easing& easing() const { return easing_; }
  void set_easing(const Easing& value) { easing_ = value; }
  const Interval& duration() const { return duration_; }
  void set_duration(const Interval& value) { duration_ = value; }
  const Interval& delay() const { return delay_; }
  void set_delay(const Interval& value) { delay_ = value; }
  const std::function<void()>& callback() const { return callback_; }
  void set_callback(const std::function<void()>& value) { callback_ = value; }

 protected:
  // Constructors
  AdaptorBase(const Easing& easing,
              const Interval& duration,
              const Interval& delay,
              const std::function<void()>& callback);
  AdaptorBase(AdaptorBase&& other) = default;

  // Updates against the local unit time
  virtual void update(double unit) = 0;

  // Data members
  Easing easing_;
  Interval duration_;
  Interval delay_;

 private:
  // Data members
  std::function<void()> callback_;
  bool running_;
  bool finished_;
  Interval started_;
};

#pragma mark - Inline Implementations

template <typename Interval>
inline AdaptorBase<Interval>::AdaptorBase(
    const Easing& easing,
    const Interval& duration,
    const Interval& delay,
    const std::function<void()>& callback)
    : easing_(easing),
      duration_(duration),
      delay_(delay),
      callback_(callback),
      running_(false),
      finished_(false){}

template <typename Interval>
inline AdaptorBase<Interval>::~AdaptorBase() {}

#pragma mark Controlling the adaptor

template <typename Interval>
inline void AdaptorBase<Interval>::start(const Interval& now) {
  if (!running_) {
    running_ = true;
    if (!finished_) {
      started_ = now;
    }
  }
}

template <typename Interval>
inline void AdaptorBase<Interval>::stop() {
  if (running_) {
    running_ = false;
  }
}

template <typename Interval>
inline void AdaptorBase<Interval>::update(const Interval& now, bool callback) {
  if (running_) {
    const auto elapsed = now - started_ - delay_;
    if (elapsed < duration_) {
      update(elapsed / duration_);
    } else {
      update(1.0);
      running_ = false;
      finished_ = true;
      started_ = Interval();
      if (callback && callback_) {
        callback_();
      }
    }
  }
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_ADAPTOR_BASE_H_
