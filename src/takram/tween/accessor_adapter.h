//
//  takram/tween/accessor_adapter.h
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
#include <memory>
#include <string>

#include "takram/easing.h"
#include "takram/tween/adapter_base.h"
#include "takram/tween/hash.h"
#include "takram/tween/transform.h"

namespace takram {
namespace tween {

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
class AccessorAdapter : public AdapterBase<Interval> {
 public:
  using Value = T;

  // Constructors
  AccessorAdapter(Class *target,
                  Getter getter,
                  Setter setter,
                  const std::string& name,
                  const Value& to,
                  const Easing& easing,
                  const Interval& duration,
                  const Interval& delay,
                  const std::function<void()>& callback);

  // Disallow copy and assign
  AccessorAdapter(const AccessorAdapter&) = delete;
  AccessorAdapter& operator=(const AccessorAdapter&) = delete;

  // Controlling the adapter
  void update(double unit) override;

  // Hash
  std::size_t key() const override;
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

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline AccessorAdapter<Interval, T, Class, Getter, Setter>
    ::AccessorAdapter(Class *target,
                      Getter getter,
                      Setter setter,
                      const std::string& name,
                      const Value& to,
                      const Easing& easing,
                      const Interval& duration,
                      const Interval& delay,
                      const std::function<void()>& callback)
    : AdapterBase<Interval>(easing, duration, delay, callback),
      target_(target),
      getter_(getter),
      setter_(setter),
      hash_(std::hash<std::string>()(name)),
      from_((target->*getter)()),
      to_(to) {}

#pragma mark Controlling the adapter

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline void AccessorAdapter<Interval, T, Class, Getter, Setter>
    ::update(double unit) {
  if (unit < 0.0) {
    from_ = (target_->*getter_)();
  } else if (AdapterBase<Interval>::duration_.empty() || unit > 1.0) {
    (target_->*setter_)(Transform(Easing(), 1.0, from_, to_));
  } else {
    (target_->*setter_)(Transform(Easing(), unit, from_, to_));
  }
}

#pragma mark Hash

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline std::size_t AccessorAdapter<Interval, T, Class, Getter, Setter>
    ::key() const {
  return Hash(target_);
}

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline std::size_t AccessorAdapter<Interval, T, Class, Getter, Setter>
    ::hash() const {
  return hash_;
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_ACCESSOR_ADAPTER_H_
