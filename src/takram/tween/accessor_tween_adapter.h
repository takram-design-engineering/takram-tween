//
//  takram/tween/accessor_tween_adapter.h
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
#ifndef TAKRAM_TWEEN_ACCESSOR_ADAPTER_H_
#define TAKRAM_TWEEN_ACCESSOR_ADAPTER_H_

#include <cstddef>
#include <functional>
#include <string>

#include "takram/tween/hash.h"
#include "takram/tween/transform.h"
#include "takram/tween/tween_adapter.h"

namespace takram {
namespace tween {

template <typename I, typename T, typename Easing,
          typename Class, typename Getter, typename Setter>
class AccessorAdapter : public Adapter<I> {
 public:
  using Interval = I;
  using Value = T;

  // Constructors
  AccessorAdapter(Class *target,
                       Getter getter,
                       Setter setter,
                       const std::string& name,
                       const Value& to,
                       const Interval& duration,
                       const Interval& delay,
                       const std::function<void()>& callback);
  AccessorAdapter(Class *target,
                       Getter getter,
                       Setter setter,
                       const std::string& name,
                       const Value& to,
                       typename Interval::Value duration,
                       typename Interval::Value delay,
                       const std::function<void()>& callback);

  // Disallow copy and assign
  AccessorAdapter(const AccessorAdapter&) = delete;
  AccessorAdapter& operator=(const AccessorAdapter&) = delete;

  // Controlling tween
  void update(double unit) override;

  // Hash
  std::size_t target_hash() const override;
  std::size_t hash() const override;

 private:
  // Data members
  Class *target_;
  Getter getter_;
  Setter setter_;
  std::size_t hash_;
  Value from_;
  Value to_;
};

#pragma mark - Inline Implementations

template <typename I, typename T, typename Easing,
          typename Class, typename Getter, typename Setter>
inline AccessorAdapter<I, T, Easing, Class, Getter, Setter>
    ::AccessorAdapter(Class *target,
                           Getter getter,
                           Setter setter,
                           const std::string& name,
                           const Value& to,
                           const Interval& duration,
                           const Interval& delay,
                           const std::function<void()>& callback)
    : Adapter<I>(duration, delay, callback),
      target_(target),
      getter_(getter),
      setter_(setter),
      hash_(std::hash<std::string>()(name)),
      from_((target->*getter)()),
      to_(to) {}

template <typename I, typename T, typename Easing,
          typename Class, typename Getter, typename Setter>
inline AccessorAdapter<I, T, Easing, Class, Getter, Setter>
    ::AccessorAdapter(Class *target,
                           Getter getter,
                           Setter setter,
                           const std::string& name,
                           const Value& to,
                           typename Interval::Value duration,
                           typename Interval::Value delay,
                           const std::function<void()>& callback)
    : Adapter<I>(Interval(duration), Interval(delay), callback),
      target_(target),
      getter_(getter),
      setter_(setter),
      hash_(std::hash<std::string>()(name)),
      from_((target->*getter)()),
      to_(to) {}

template <typename I, typename T, typename Easing,
          typename Class, typename Getter, typename Setter>
inline void AccessorAdapter<I, T, Easing, Class, Getter, Setter>
    ::update(double unit) {
  // TODO
}

template <typename I, typename T, typename Easing,
          typename Class, typename Getter, typename Setter>
inline std::size_t AccessorAdapter<I, T, Easing, Class, Getter, Setter>
    ::target_hash() const {
  return Hash(target_);
}

template <typename I, typename T, typename Easing,
          typename Class, typename Getter, typename Setter>
inline std::size_t AccessorAdapter<I, T, Easing, Class, Getter, Setter>
    ::hash() const {
  return hash_;
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_ACCESSOR_ADAPTER_H_
