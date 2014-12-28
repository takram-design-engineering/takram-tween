//
//  takram/tween/pointer_adaptor.h
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
#ifndef TAKRAM_TWEEN_POINTER_ADAPTOR_H_
#define TAKRAM_TWEEN_POINTER_ADAPTOR_H_

#include <cassert>
#include <cstddef>
#include <functional>

#include "takram/easing.h"
#include "takram/tween/adaptor_base.h"
#include "takram/tween/hash.h"
#include "takram/tween/transform.h"

namespace takram {
namespace tween {

template <typename Interval_, typename T>
class PointerAdaptor : public AdaptorBase<Interval_> {
 public:
  using Interval = Interval_;
  using Value = T;

  // Constructors
  PointerAdaptor(T *target,
                 const Value& to,
                 const Easing& easing,
                 const Interval& duration,
                 const Interval& delay,
                 const std::function<void()>& callback);
  PointerAdaptor(PointerAdaptor&& other) = default;

  // Disallow copy and assign
  PointerAdaptor(const PointerAdaptor&) = delete;
  PointerAdaptor& operator=(const PointerAdaptor&) = delete;

  // Hash
  std::size_t object_hash() const override;
  std::size_t target_hash() const override;

  // Parameters
  T * target() const { return target_; }
  const Value& from() const { return from_; }
  const Value& to() const { return to_; }

 protected:
  // Updates against the local unit time
  void update(double unit) override;

 private:
  // Data members
  T *target_;
  Value from_;
  Value to_;
};

#pragma mark - Inline Implementations

template <typename Interval, typename T>
inline PointerAdaptor<Interval, T>::PointerAdaptor(
    T *target,
    const Value& to,
    const Easing& easing,
    const Interval& duration,
    const Interval& delay,
    const std::function<void()>& callback)
    : AdaptorBase<Interval>(easing, duration, delay, callback),
      target_(target),
      from_(*target),
      to_(to) {}

#pragma mark Updates against the local unit time

template <typename Interval, typename T>
inline void PointerAdaptor<Interval, T>::update(double unit) {
  assert(target_);
  if (unit < 0.0) {
    from_ = *target_;
  } else if (this->duration_.empty() || unit > 1.0) {
    *target_ = Transform(this->easing_, 1.0, from_, to_);
  } else {
    *target_ = Transform(this->easing_, unit, from_, to_);
  }
}

#pragma mark Hash

template <typename Interval, typename T>
inline std::size_t PointerAdaptor<Interval, T>::object_hash() const {
  return Hash(target_);
}

template <typename Interval, typename T>
inline std::size_t PointerAdaptor<Interval, T>::target_hash() const {
  return Hash(target_);
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_POINTER_ADAPTOR_H_
