//
//  takram/tween/adaptor.h
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
#ifndef TAKRAM_TWEEN_ADAPTOR_H_
#define TAKRAM_TWEEN_ADAPTOR_H_

#include <cstddef>

#include "takram/tween/easing.h"
#include "takram/tween/types.h"

namespace takram {
namespace tween {

template <class Interval_>
class Adaptor {
 public:
  using Interval = Interval_;

 public:
  virtual ~Adaptor() = 0;

  // Disallow copy semantics
  Adaptor(const Adaptor& other) = delete;
  Adaptor& operator=(const Adaptor& other) = delete;

  // Controlling the adaptor
  void start(const Interval& now);
  void stop();
  void update(const Interval& now, bool callback = true);

  // Attributes
  bool running() const { return running_; }
  bool finished() const { return finished_; }

  // Hash
  virtual std::size_t object_hash() const = 0;
  virtual std::size_t target_hash() const = 0;

  // Parameters
  const Easing& easing() const;
  void set_easing(const Easing& value);
  const Interval& duration() const;
  void set_duration(const Interval& value);
  const Interval& delay() const;
  void set_delay(const Interval& value);
  const Callback& callback() const;
  void set_callback(const Callback& value);

 protected:
  // Constructors
  Adaptor(const Easing& easing,
          const Interval& duration,
          const Interval& delay,
          const Callback& callback);

  // Move semantics
  Adaptor(Adaptor&& other) = default;

  // Updates against the local unit time
  virtual void update(Unit unit) = 0;

 private:
  Easing easing_;
  Interval duration_;
  Interval delay_;
  Callback callback_;
  bool running_;
  bool finished_;
  Interval started_;
};

#pragma mark -

template <class Interval>
inline Adaptor<Interval>::Adaptor(
    const Easing& easing,
    const Interval& duration,
    const Interval& delay,
    const Callback& callback)
    : easing_(easing),
      duration_(duration),
      delay_(delay),
      callback_(callback),
      running_(false),
      finished_(false) {}

template <class Interval>
inline Adaptor<Interval>::~Adaptor() {}

#pragma mark Controlling the adaptor

template <class Interval>
inline void Adaptor<Interval>::start(const Interval& now) {
  if (!running_) {
    running_ = true;
    if (!finished_) {
      started_ = now;
    }
  }
}

template <class Interval>
inline void Adaptor<Interval>::stop() {
  if (running_) {
    running_ = false;
  }
}

template <class Interval>
inline void Adaptor<Interval>::update(const Interval& now, bool callback) {
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

#pragma mark Parameters

template <class Interval>
inline const Easing& Adaptor<Interval>::easing() const {
  return easing_;
}

template <class Interval>
inline void Adaptor<Interval>::set_easing(const Easing& value) {
  easing_ = value;
}

template <class Interval>
inline const Interval& Adaptor<Interval>::duration() const {
  return duration_;
}

template <class Interval>
inline void Adaptor<Interval>::set_duration(const Interval& value) {
  duration_ = value;
}

template <class Interval>
inline const Interval& Adaptor<Interval>::delay() const {
  return delay_;
}

template <class Interval>
inline void Adaptor<Interval>::set_delay(const Interval& value) {
  delay_ = value;
}

template <class Interval>
inline const Callback& Adaptor<Interval>::callback() const {
  return callback_;
}

template <class Interval>
inline void Adaptor<Interval>::set_callback(const Callback& value) {
  callback_ = value;
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_ADAPTOR_H_
