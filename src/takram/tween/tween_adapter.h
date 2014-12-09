//
//  takram/tween/tween_adapter.h
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
#ifndef TAKRAM_TWEEN_ADAPTER_H_
#define TAKRAM_TWEEN_ADAPTER_H_

#include <cstddef>
#include <functional>

#include "takram/tween/interval.h"

namespace takram {
namespace tween {

template <typename Interval>
class Adapter {
 public:
  // Constructors
  virtual ~Adapter() = 0;

  // Disallow copy and assign
  Adapter(const Adapter&) = delete;
  Adapter& operator=(const Adapter&) = delete;

  // Controlling tween
  void start(const Interval& time);
  void stop(const Interval& time);
  void update(const Interval& time);
  virtual void update(double unit) = 0;

  // Properties
  bool running() const { return running_; }
  bool finished() const { return finished_; }

  // Hash
  virtual std::size_t target_hash() const = 0;
  virtual std::size_t hash() const = 0;

  // Parameters
  const Interval& duration() const { return duration_; }
  void set_duration(const Interval& value) { duration_ = value; }
  const Interval& delay() const { return delay_; }
  void set_delay(const Interval& value) { delay_ = value; }
  std::function<void()> callback() const { return callback_; }
  void set_callback(const std::function<void()>& value) { callback_ = value; }

 protected:
  // Constructors
  Adapter(const Interval& duration,
               const Interval& delay,
               const std::function<void()>& callback);

  // Data members
  Interval duration_;
  Interval delay_;
  std::function<void()> callback_;

 private:
  // Data members
  bool running_;
  bool finished_;
  Interval started_time_;
};

#pragma mark - Inline Implementations

template <typename Interval>
inline Adapter<Interval>::Adapter(
    const Interval& duration,
    const Interval& delay,
    const std::function<void()>& callback)
    : duration_(duration),
      delay_(delay),
      callback_(callback),
      running_(false),
      finished_(false) {}

template <typename Interval>
inline Adapter<Interval>::~Adapter() {}

#pragma mark Controlling tween

template <typename Interval>
inline void Adapter<Interval>::start(const Interval& time) {
  if (!running_) {
    running_ = true;
    started_time_ = time;
  }
}

template <typename Interval>
inline void Adapter<Interval>::stop(const Interval& time) {
  if (running_) {
    running_ = false;
    started_time_ = Interval();
  }
}

template <typename Interval>
inline void Adapter<Interval>::update(const Interval& time) {
  if (running_) {
    const auto elapsed = time - started_time_ - delay_;
    if (elapsed <= duration_) {
      update(time / duration_);
    } else {
      update(1.0);
      finished_ = true;
      running_ = false;
      if (callback_) {
        callback_();
      }
    }
  }
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_ADAPTER_H_
