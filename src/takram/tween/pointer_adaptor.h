//
//  takram/tween/pointer_adaptor.h
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
#ifndef TAKRAM_TWEEN_POINTER_ADAPTOR_H_
#define TAKRAM_TWEEN_POINTER_ADAPTOR_H_

#include <cassert>
#include <cstddef>

#include "takram/tween/adaptor.h"
#include "takram/tween/easing.h"
#include "takram/tween/hash.h"
#include "takram/tween/transform.h"
#include "takram/tween/types.h"

namespace takram {
namespace tween {

template <class Interval_, class Value_>
class PointerAdaptor : public Adaptor<Interval_> {
 public:
  using Interval = Interval_;
  using Value = Value_;

 public:
  template <class T>
  PointerAdaptor(Value *target,
                 const T& to,
                 const Easing& easing,
                 const Interval& duration,
                 const Interval& delay,
                 const Callback& callback);

  // Disallow copy semantics
  PointerAdaptor(const PointerAdaptor&) = delete;
  PointerAdaptor& operator=(const PointerAdaptor&) = delete;

  // Move semantics
  PointerAdaptor(PointerAdaptor&&) = default;

  // Controlling the adaptor
  using Adaptor<Interval>::update;

  // Hash
  std::size_t object_hash() const override;
  std::size_t target_hash() const override;

  // Parameters
  Value * target() const { return target_; }
  const Value& from() const { return from_; }
  const Value& to() const { return to_; }

 protected:
  // Updates against the local unit time
  void update(Unit unit) override;

 private:
  // Data members
  Value *target_;
  Value from_;
  Value to_;
};

#pragma mark -

template <class Interval, class Value>
template <class T>
inline PointerAdaptor<Interval, Value>::PointerAdaptor(
    Value *target,
    const T& to,
    const Easing& easing,
    const Interval& duration,
    const Interval& delay,
    const Callback& callback)
    : Adaptor<Interval>(easing, duration, delay, callback),
      target_(target),
      from_(*target),
      to_(to) {}

#pragma mark Updates against the local unit time

template <class Interval, class Value>
inline void PointerAdaptor<Interval, Value>::update(Unit unit) {
  assert(target_);
  if (unit < 0.0) {
    from_ = *target_;
  } else if (this->duration().empty() || unit > 1.0) {
    *target_ = Transform(this->easing(), 1.0, from_, to_);
  } else {
    *target_ = Transform(this->easing(), unit, from_, to_);
  }
}

#pragma mark Hash

template <class Interval, class Value>
inline std::size_t PointerAdaptor<Interval, Value>::object_hash() const {
  return Hash(target_);
}

template <class Interval, class Value>
inline std::size_t PointerAdaptor<Interval, Value>::target_hash() const {
  return Hash(target_);
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_POINTER_ADAPTOR_H_
