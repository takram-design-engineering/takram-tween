//
//  takram/tween/tween.h
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
#ifndef TAKRAM_TWEEN_TWEEN_H_
#define TAKRAM_TWEEN_TWEEN_H_

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>

#include "takram/tween/accessor_adaptor.h"
#include "takram/tween/adaptor.h"
#include "takram/tween/interval.h"
#include "takram/tween/pointer_adaptor.h"
#include "takram/tween/types.h"

namespace takram {
namespace tween {

template <class Interval>
class Timeline;

template <class Interval_ = Time>
class Tween final {
 public:
  using Interval = Interval_;
  using Timeline = Timeline<Interval>;

 private:
  // For resolving ambiguity on platform where the std::function's constructor
  // that takes single template argument is not specified as explicit.
  template <class T>
  using EnableIfCallback = typename std::enable_if<
    !std::is_convertible<T, Interval>::value
  >::type;

 public:
  Tween();
  explicit Tween(const std::shared_ptr<Adaptor<Interval>>& adaptor,
                 Timeline *timeline = nullptr);

  // Construct with pointer
  template <class T, class Value>
  Tween(Value *target,
        const T& to,
        const Easing& easing,
        const Interval& duration,
        Timeline *timeline = nullptr);
  template <class T, class Value>
  Tween(Value *target,
        const T& to,
        const Easing& easing,
        const Interval& duration,
        const Interval& delay,
        Timeline *timeline = nullptr);
  template <
    class T, class Value, class Callback,
    EnableIfCallback<Callback> * = nullptr
  >
  Tween(Value *target,
        const T& to,
        const Easing& easing,
        const Interval& duration,
        const Callback& callback,
        Timeline *timeline = nullptr);
  template <class T, class Value>
  Tween(Value *target,
        const T& to,
        const Easing& easing,
        const Interval& duration,
        const Interval& delay,
        const Callback& callback,
        Timeline *timeline = nullptr);

  // Construct with accessor
  template <class Value, class Class, class Getter, class Setter>
  Tween(Class *target,
        Getter getter,
        Setter setter,
        const std::string& name,
        const Value& to,
        const Easing& easing,
        const Interval& duration,
        Timeline *timeline = nullptr);
  template <class Value, class Class, class Getter, class Setter>
  Tween(Class *target,
        Getter getter,
        Setter setter,
        const std::string& name,
        const Value& to,
        const Easing& easing,
        const Interval& duration,
        const Interval& delay,
        Timeline *timeline = nullptr);
  template <
    class Value, class Class, class Getter, class Setter,
    class Callback, EnableIfCallback<Callback> * = nullptr
  >
  Tween(Class *target,
        Getter getter,
        Setter setter,
        const std::string& name,
        const Value& to,
        const Easing& easing,
        const Interval& duration,
        const Callback& callback,
        Timeline *timeline = nullptr);
  template <class Value, class Class, class Getter, class Setter>
  Tween(Class *target,
        Getter getter,
        Setter setter,
        const std::string& name,
        const Value& to,
        const Easing& easing,
        const Interval& duration,
        const Interval& delay,
        const Callback& callback,
        Timeline *timeline = nullptr);

  // Copy semantics
  Tween(const Tween&) = default;
  Tween& operator=(const Tween&) = default;

  // Comparison
  bool operator==(const Tween& other) const;
  bool operator!=(const Tween& other) const;

  // Controlling tween
  void start();
  void stop();

  // Attributes
  bool running() const;
  bool finished() const;
  bool empty() const;

  // Parameters
  const Easing& easing() const;
  void set_easing(const Easing& value);
  const Interval& duration() const;
  void set_duration(const Interval& value);
  const Interval& delay() const;
  void set_delay(const Interval& value);
  const Callback& callback() const;
  void set_callback(const Callback& value);

  // Timeline
  Timeline * timeline() const;
  void set_timeline(Timeline *value);

  // Adaptor
  std::shared_ptr<Adaptor<Interval>> adaptor() const { return adaptor_; }

  // Conversion
  operator bool() const { return adaptor_ && timeline_; }

 private:
  std::shared_ptr<Adaptor<Interval>> adaptor_;
  Timeline *timeline_;
};

#pragma mark -

template <class Interval>
inline Tween<Interval>::Tween()
    : timeline_() {}

template <class Interval>
inline Tween<Interval>::Tween(const std::shared_ptr<Adaptor<Interval>>& adaptor,
                              Timeline *timeline)
    : adaptor_(adaptor),
      timeline_(timeline) {}

#pragma mark Construct with pointer

template <class Interval>
template <class T, class Value>
inline Tween<Interval>::Tween(Value *target,
                              const T& to,
                              const Easing& easing,
                              const Interval& duration,
                              Timeline *timeline)
    : Tween(target, to, easing, duration, Interval(), nullptr, timeline) {}

template <class Interval>
template <class T, class Value>
inline Tween<Interval>::Tween(Value *target,
                              const T& to,
                              const Easing& easing,
                              const Interval& duration,
                              const Interval& delay,
                              Timeline *timeline)
    : Tween(target, to, easing, duration, delay, nullptr, timeline) {}

template <class Interval>
template <
  class T, class Value, class Callback,
  typename Tween<Interval>::template EnableIfCallback<Callback> *
>
inline Tween<Interval>::Tween(Value *target,
                              const T& to,
                              const Easing& easing,
                              const Interval& duration,
                              const Callback& callback,
                              Timeline *timeline)
    : Tween(target, to, easing, duration, Interval(), callback, timeline) {}

template <class Interval>
template <class T, class Value>
inline Tween<Interval>::Tween(Value *target,
                              const T& to,
                              const Easing& easing,
                              const Interval& duration,
                              const Interval& delay,
                              const Callback& callback,
                              Timeline *timeline)
    : adaptor_(std::make_shared<PointerAdaptor<Interval, Value>>(
          target, to, easing, duration, delay, callback)),
      timeline_(timeline) {
  assert(target);
}

#pragma mark Construct with accessor

template <class Interval>
template <class Value, class Class, class Getter, class Setter>
inline Tween<Interval>::Tween(Class *target,
                              Getter getter,
                              Setter setter,
                              const std::string& name,
                              const Value& to,
                              const Easing& easing,
                              const Interval& duration,
                              Timeline *timeline)
    : Tween(target, getter, setter, name, to,
            easing, Interval(duration), Interval(), nullptr, timeline) {}

template <class Interval>
template <class Value, class Class, class Getter, class Setter>
inline Tween<Interval>::Tween(Class *target,
                              Getter getter,
                              Setter setter,
                              const std::string& name,
                              const Value& to,
                              const Easing& easing,
                              const Interval& duration,
                              const Interval& delay,
                              Timeline *timeline)
    : Tween(target, getter, setter, name, to,
            easing, Interval(duration), Interval(delay), nullptr, timeline) {}

template <class Interval>
template <
  class Value, class Class, class Getter, class Setter, class Callback,
  typename Tween<Interval>::template EnableIfCallback<Callback> *
>
inline Tween<Interval>::Tween(Class *target,
                              Getter getter,
                              Setter setter,
                              const std::string& name,
                              const Value& to,
                              const Easing& easing,
                              const Interval& duration,
                              const Callback& callback,
                              Timeline *timeline)
    : Tween(target, getter, setter, name, to,
            easing, Interval(duration), Interval(), callback, timeline) {}

template <class Interval>
template <class Value, class Class, class Getter, class Setter>
inline Tween<Interval>::Tween(Class *target,
                              Getter getter,
                              Setter setter,
                              const std::string& name,
                              const Value& to,
                              const Easing& easing,
                              const Interval& duration,
                              const Interval& delay,
                              const Callback& callback,
                              Timeline *timeline)
    : adaptor_(std::make_shared<AccessorAdaptor<
          Interval, Value, Class, Getter, Setter>>(
              target, getter, setter, name, to,
              easing, duration, delay, callback)),
      timeline_(timeline) {
  assert(target);
}

#pragma mark Comparison

template <class Interval>
inline bool Tween<Interval>::operator==(const Tween& other) const {
  return adaptor_ == other.adaptor_ && timeline_ == other.timeline_;
}

template <class Interval>
inline bool Tween<Interval>::operator!=(const Tween& other) const {
  return !operator==(other);
}

#pragma mark Attributes

template <class Interval>
inline bool Tween<Interval>::running() const {
  return adaptor_ && adaptor_->running();
}

template <class Interval>
inline bool Tween<Interval>::finished() const {
  return adaptor_ && adaptor_->finished();
}

template <class Interval>
inline bool Tween<Interval>::empty() const {
  return !adaptor_;
}

#pragma mark Parameters

template <class Interval>
inline const Easing& Tween<Interval>::easing() const {
  assert(adaptor_);
  return adaptor_->easing();
}

template <class Interval>
inline void Tween<Interval>::set_easing(const Easing& value) {
  assert(adaptor_);
  adaptor_->set_easing(value);
}

template <class Interval>
inline const Interval& Tween<Interval>::duration() const {
  assert(adaptor_);
  return adaptor_->duration();
}

template <class Interval>
inline void Tween<Interval>::set_duration(const Interval& value) {
  assert(adaptor_);
  return adaptor_->set_duration(value);
}

template <class Interval>
inline const Interval& Tween<Interval>::delay() const {
  assert(adaptor_);
  return adaptor_->delay();
}

template <class Interval>
inline void Tween<Interval>::set_delay(const Interval& value) {
  assert(adaptor_);
  return adaptor_->set_delay(value);
}

template <class Interval>
inline const Callback& Tween<Interval>::callback() const {
  assert(adaptor_);
  return adaptor_->callback();
}

template <class Interval>
inline void Tween<Interval>::set_callback(const Callback& value) {
  return adaptor_->set_callback(value);
}

#pragma mark Timeline

template <class Interval>
inline Timeline<Interval> * Tween<Interval>::timeline() const {
  return timeline_;
}

template <class Interval>
inline void Tween<Interval>::set_timeline(Timeline *value) {
  timeline_ = value;
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TWEEN_H_
