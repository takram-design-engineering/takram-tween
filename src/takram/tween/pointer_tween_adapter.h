//
//  takram/tween/pointer_tween_adapter.h
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
#ifndef TAKRAM_TWEEN_POINTER_ADAPTER_H_
#define TAKRAM_TWEEN_POINTER_ADAPTER_H_

#include <cstddef>
#include <functional>

#include "takram/tween/hash.h"
#include "takram/tween/transform.h"
#include "takram/tween/tween_adapter.h"

namespace takram {
namespace tween {

template <typename I, typename T, typename Easing>
class PointerAdapter : public Adapter<I> {
 public:
  using Interval = I;
  using Value = T;

  // Constructors
  PointerAdapter(T *target,
                      const Value& to,
                      const Interval& duration,
                      const Interval& delay,
                      const std::function<void()>& callback);
  PointerAdapter(T *target,
                      const Value& to,
                      typename Interval::Value duration,
                      typename Interval::Value delay,
                      const std::function<void()>& callback);
  
  // Disallow copy and assign
  PointerAdapter(const PointerAdapter&) = delete;
  PointerAdapter& operator=(const PointerAdapter&) = delete;

  // Controlling tween
  void update(double unit) override;

  // Hash
  std::size_t target_hash() const override;
  std::size_t hash() const override;

 private:
  // Data members
  T *target_;
  Value from_;
  Value to_;
};

#pragma mark - Inline Implementations

template <typename I, typename T, typename Easing>
inline PointerAdapter<I, T, Easing>::PointerAdapter(
    T *target,
    const Value& to,
    const Interval& duration,
    const Interval& delay,
    const std::function<void()>& callback)
    : Adapter<Interval>(duration, delay, callback),
      target_(target),
      from_(*target),
      to_(to) {}

template <typename I, typename T, typename Easing>
inline PointerAdapter<I, T, Easing>::PointerAdapter(
    T *target,
    const Value& to,
    typename Interval::Value duration,
    typename Interval::Value delay,
    const std::function<void()>& callback)
    : Adapter<Interval>(Interval(duration), Interval(delay), callback),
      target_(target),
      from_(*target),
      to_(to) {}

#pragma mark Controlling tween

template <typename I, typename T, typename Easing>
inline void PointerAdapter<I, T, Easing>::update(double unit) {
  // TODO
}

#pragma mark Hash

template <typename I, typename T, typename Easing>
inline std::size_t PointerAdapter<I, T, Easing>::target_hash() const {
  return Hash(target_);
}

template <typename I, typename T, typename Easing>
inline std::size_t PointerAdapter<I, T, Easing>::hash() const {
  return Hash(target_);
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_POINTER_ADAPTER_H_
