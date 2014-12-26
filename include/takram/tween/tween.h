//
//  takram/tween/tween.h
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
#ifndef TAKRAM_TWEEN_TWEEN_H_
#define TAKRAM_TWEEN_TWEEN_H_

#include <cassert>
#include <functional>
#include <memory>
#include <string>

#include "glog/logging.h"

#include "takram/easing.h"
#include "takram/tween/accessor_adaptor.h"
#include "takram/tween/adaptor_base.h"
#include "takram/tween/interval.h"
#include "takram/tween/pointer_adaptor.h"
#include "takram/tween/timeline.h"

namespace takram {
namespace tween {

template <typename Interval = Time>
class Tween final {
 public:
  using Timeline = Timeline<Interval>;
  using IntervalValue = typename Interval::Value;

  // Constructors
  Tween();
  Tween(const Tween& other);

  // Construct with pointer
  template <typename T, typename U>
  Tween(T *target,
        const U& to,
        const Easing& easing,
        IntervalValue duration,
        Timeline *timeline = nullptr);
  template <typename T, typename U>
  Tween(T *target,
        const U& to,
        const Easing& easing,
        IntervalValue duration,
        IntervalValue delay,
        Timeline *timeline = nullptr);
  template <typename T, typename U>
  Tween(T *target,
        const U& to,
        const Easing& easing,
        IntervalValue duration,
        const std::function<void()>& callback,
        Timeline *timeline = nullptr);
  template <typename T, typename U>
  Tween(T *target,
        const U& to,
        const Easing& easing,
        IntervalValue duration,
        IntervalValue delay,
        const std::function<void()>& callback,
        Timeline *timeline = nullptr);

  // Construct with accessor
  template <typename Class, typename Getter, typename Setter, typename T>
  Tween(Class *target,
        Getter getter,
        Setter setter,
        const std::string& name,
        const T& to,
        const Easing& easing,
        IntervalValue duration,
        Timeline *timeline = nullptr);
  template <typename Class, typename Getter, typename Setter, typename T>
  Tween(Class *target,
        Getter getter,
        Setter setter,
        const std::string& name,
        const T& to,
        const Easing& easing,
        IntervalValue duration,
        IntervalValue delay,
        Timeline *timeline = nullptr);
  template <typename Class, typename Getter, typename Setter, typename T>
  Tween(Class *target,
        Getter getter,
        Setter setter,
        const std::string& name,
        const T& to,
        const Easing& easing,
        IntervalValue duration,
        const std::function<void()>& callback,
        Timeline *timeline = nullptr);
  template <typename Class, typename Getter, typename Setter, typename T>
  Tween(Class *target,
        Getter getter,
        Setter setter,
        const std::string& name,
        const T& to,
        const Easing& easing,
        IntervalValue duration,
        IntervalValue delay,
        const std::function<void()>& callback,
        Timeline *timeline = nullptr);

  // Assignment
  Tween& operator=(const Tween& other);

  // Comparison
  bool operator==(const Tween& other) const;
  bool operator!=(const Tween& other) const { return !operator==(other); }

  // Controlling tween
  void start();
  void stop();

  // Properties
  bool running() const;
  bool finished() const;
  bool empty() const;

  // Parameters
  IntervalValue duration() const;
  void set_duration(IntervalValue value);
  IntervalValue delay() const;
  void set_delay(IntervalValue value);
  std::function<void()> callback() const;
  void set_callback(const std::function<void()>& value);

  // Timeline
  Timeline * timeline() const { return timeline_; }
  void set_timeline(Timeline *value);

 protected:
  // Initializers
  template <typename T, typename U>
  void init(T *target,
            const U& to,
            const Easing& easing,
            const Interval& duration,
            const Interval& delay,
            const std::function<void()>& callback);
  template <typename T, typename Class, typename Getter, typename Setter>
  void init(Class *target,
            Getter getter,
            Setter setter,
            const std::string& name,
            const T& to,
            const Easing& easing,
            const Interval& duration,
            const Interval& delay,
            const std::function<void()>& callback);

 private:
  // Data members
  std::shared_ptr<AdaptorBase<Interval>> adaptor_;
  Timeline *timeline_;
};

#pragma mark - Inline Implementations

template <typename Interval>
inline Tween<Interval>::Tween()
    : timeline_(nullptr) {}

template <typename Interval>
inline Tween<Interval>::Tween(const Tween& other)
    : adaptor_(other.adaptor_),
      timeline_(other.timeline_) {}

#pragma mark Construct with pointer

template <typename Interval>
template <typename T, typename U>
inline Tween<Interval>::Tween(T *target,
                              const U& to,
                              const Easing& easing,
                              IntervalValue duration,
                              Timeline *timeline)
    : timeline_(timeline) {
  init(target, to, easing, Interval(duration), Interval(), nullptr);
}

template <typename Interval>
template <typename T, typename U>
inline Tween<Interval>::Tween(T *target,
                              const U& to,
                              const Easing& easing,
                              IntervalValue duration,
                              IntervalValue delay,
                              Timeline *timeline)
    : timeline_(timeline) {
  init(target, to, easing, Interval(duration), Interval(delay), nullptr);
}

template <typename Interval>
template <typename T, typename U>
inline Tween<Interval>::Tween(T *target,
                              const U& to,
                              const Easing& easing,
                              IntervalValue duration,
                              const std::function<void()>& callback,
                              Timeline *timeline)
    : timeline_(timeline) {
  init(target, to, easing, Interval(duration), Interval(), callback);
}

template <typename Interval>
template <typename T, typename U>
inline Tween<Interval>::Tween(T *target,
                              const U& to,
                              const Easing& easing,
                              IntervalValue duration,
                              IntervalValue delay,
                              const std::function<void()>& callback,
                              Timeline *timeline)
    : timeline_(timeline) {
  init(target, to, easing, Interval(duration), Interval(delay), callback);
}

#pragma mark Construct with accessor

template <typename Interval>
template <typename Class, typename Getter, typename Setter, typename T>
inline Tween<Interval>::Tween(Class *target,
                              Getter getter,
                              Setter setter,
                              const std::string& name,
                              const T& to,
                              const Easing& easing,
                              IntervalValue duration,
                              Timeline *timeline)
    : timeline_(timeline) {
  init(target, getter, setter, name, to,
       easing, Interval(duration), Interval(), nullptr);
}

template <typename Interval>
template <typename Class, typename Getter, typename Setter, typename T>
inline Tween<Interval>::Tween(Class *target,
                              Getter getter,
                              Setter setter,
                              const std::string& name,
                              const T& to,
                              const Easing& easing,
                              IntervalValue duration,
                              IntervalValue delay,
                              Timeline *timeline)
    : timeline_(timeline) {
  init(target, getter, setter, name, to,
       easing, Interval(duration), Interval(delay), nullptr);
}

template <typename Interval>
template <typename Class, typename Getter, typename Setter, typename T>
inline Tween<Interval>::Tween(Class *target,
                              Getter getter,
                              Setter setter,
                              const std::string& name,
                              const T& to,
                              const Easing& easing,
                              IntervalValue duration,
                              const std::function<void()>& callback,
                              Timeline *timeline)
    : timeline_(timeline) {
  init(target, getter, setter, name, to,
       easing, Interval(duration), Interval(), callback);
}

template <typename Interval>
template <typename Class, typename Getter, typename Setter, typename T>
inline Tween<Interval>::Tween(Class *target,
                              Getter getter,
                              Setter setter,
                              const std::string& name,
                              const T& to,
                              const Easing& easing,
                              IntervalValue duration,
                              IntervalValue delay,
                              const std::function<void()>& callback,
                              Timeline *timeline)
    : timeline_(timeline) {
  init(target, getter, setter, name, to,
       easing, Interval(duration), Interval(delay), callback);
}

#pragma mark Assignment

template <typename Interval>
inline Tween<Interval>& Tween<Interval>::operator=(
    const Tween& other) {
  if (&other != this) {
    adaptor_ = other.adaptor_;
    timeline_ = other.timeline_;
  }
  return *this;
}

#pragma mark Comparison

template <typename Interval>
inline bool Tween<Interval>::operator==(const Tween& other) const {
  return adaptor_ == other.adaptor_ && timeline_ == other.timeline_;
}

#pragma mark Initializers

template <typename Interval>
template <typename T, typename U>
inline void Tween<Interval>::init(
    T *target,
    const U& to,
    const Easing& easing,
    const Interval& duration,
    const Interval& delay,
    const std::function<void()>& callback) {
  assert(target);
  using Adaptor = PointerAdaptor<Interval, T>;
  adaptor_ = std::make_shared<Adaptor>(
      target, to, easing, duration, delay, callback);
}

template <typename Interval>
template <typename T, typename Class, typename Getter, typename Setter>
inline void Tween<Interval>::init(
    Class *target,
    Getter getter,
    Setter setter,
    const std::string& name,
    const T& to,
    const Easing& easing,
    const Interval& duration,
    const Interval& delay,
    const std::function<void()>& callback) {
  assert(target);
  using Adaptor = AccessorAdaptor<Interval, T, Class, Getter, Setter>;
  adaptor_ = std::make_shared<Adaptor>(
      target, getter, setter, name, to, easing, duration, delay, callback);
}

#pragma mark Controlling tween

template <typename Interval>
inline void Tween<Interval>::start() {
  if (!timeline_) {
    LOG(ERROR) << "Attempt to control tween before setting timeline.";
  } else if (!adaptor_) {
    LOG(ERROR) << "Attempt to control tween without adaptor.";
  } else if (!adaptor_->running()) {
    timeline_->add(adaptor_);
    adaptor_->start(timeline_->now());
  }
}

template <typename Interval>
inline void Tween<Interval>::stop() {
  if (!timeline_) {
    LOG(ERROR) << "Attempt to control tween before setting timeline.";
  } else if (!adaptor_) {
    LOG(ERROR) << "Attempt to control tween without adaptor.";
  } else if (adaptor_->running()) {
    adaptor_->stop();
    timeline_->remove(adaptor_);
  }
}

#pragma mark Properties

template <typename Interval>
inline bool Tween<Interval>::running() const {
  return adaptor_ && adaptor_->running();
}

template <typename Interval>
inline bool Tween<Interval>::finished() const {
  return adaptor_ && adaptor_->finished();
}

template <typename Interval>
inline bool Tween<Interval>::empty() const {
  return !adaptor_;
}

#pragma mark Parameters

template <typename Interval>
inline typename Interval::Value Tween<Interval>::duration() const {
  if (adaptor_) {
    return adaptor_->duration().count();
  } else {
    LOG(ERROR) << "Attempt to access parameter without adaptor.";
  }
  return Interval().count();
}

template <typename Interval>
inline void Tween<Interval>::set_duration(IntervalValue value) {
  if (adaptor_) {
    return adaptor_->set_duration(Interval(value));
  } else {
    LOG(ERROR) << "Attempt to change parameter without adaptor.";
  }
}

template <typename Interval>
inline typename Interval::Value Tween<Interval>::delay() const {
  if (adaptor_) {
    return adaptor_->delay().count();
  } else {
    LOG(ERROR) << "Attempt to access parameter without adaptor.";
  }
  return Interval().count();
}

template <typename Interval>
inline void Tween<Interval>::set_delay(IntervalValue value) {
  if (adaptor_) {
    return adaptor_->set_delay(Interval(value));
  } else {
    LOG(ERROR) << "Attempt to change parameter without adaptor.";
  }
}

template <typename Interval>
inline std::function<void()> Tween<Interval>::callback() const {
  if (adaptor_) {
    return adaptor_->callback();
  } else {
    LOG(ERROR) << "Attempt to access parameter without adaptor.";
  }
  return std::function<void()>();
}

template <typename Interval>
inline void Tween<Interval>::set_callback(const std::function<void()>& value) {
  if (adaptor_) {
    return adaptor_->set_callback(value);
  } else {
    LOG(ERROR) << "Attempt to change parameter without adaptor.";
  }
}

#pragma mark Timeline

template <typename Interval>
inline void Tween<Interval>::set_timeline(Timeline *value) {
  if (adaptor_ && !adaptor_->running()) {
    timeline_ = value;
  } else {
    LOG(ERROR) << "Attempt to change timeline while tween is running.";
  }
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TWEEN_H_
