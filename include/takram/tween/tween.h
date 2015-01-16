//
//  takram/tween/tween.h
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
#ifndef TAKRAM_TWEEN_TWEEN_H_
#define TAKRAM_TWEEN_TWEEN_H_

#include <cassert>
#include <functional>
#include <memory>
#include <string>

#include "takram/easing.h"
#include "takram/tween/accessor_adaptor.h"
#include "takram/tween/adaptor_base.h"
#include "takram/tween/interval.h"
#include "takram/tween/pointer_adaptor.h"
#include "takram/tween/timeline.h"

namespace takram {
namespace tween {

template <typename Interval_ = Time>
class Tween final {
 public:
  using Interval = Interval_;
  using Timeline = Timeline<Interval>;
  using IntervalValue = typename Interval::Value;

  // Constructors
  Tween();
  explicit Tween(const std::shared_ptr<AdaptorBase<Interval>>& adaptor,
                 Timeline *timeline = nullptr);
  Tween(const Tween& other) = default;

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
  Tween& operator=(const Tween& other) = default;

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
  const Easing& easing() const;
  void set_easing(const Easing& value);
  IntervalValue duration() const;
  void set_duration(IntervalValue value);
  IntervalValue delay() const;
  void set_delay(IntervalValue value);
  std::function<void()> callback() const;
  void set_callback(const std::function<void()>& value);

  // Timeline
  Timeline * timeline() const { return timeline_; }
  void set_timeline(Timeline *value);

  // Conversion
  operator bool() const { return adaptor_ && timeline_; }

 private:
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

  // Aggregations
  std::shared_ptr<AdaptorBase<Interval>> adaptor_;
  Timeline *timeline_;

  // Static storage
  static Easing null_easing_;
};

#pragma mark - Inline Implementations

template <typename Interval>
Easing Tween<Interval>::null_easing_;

template <typename Interval>
inline Tween<Interval>::Tween()
    : timeline_(nullptr) {}

template <typename Interval>
inline Tween<Interval>::Tween(
    const std::shared_ptr<AdaptorBase<Interval>>& adaptor,
    Timeline *timeline)
    : adaptor_(adaptor),
      timeline_(timeline) {}

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

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TWEEN_H_
