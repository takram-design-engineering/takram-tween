//
//  takram/tween/accessor_adaptor.h
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
#ifndef TAKRAM_TWEEN_ACCESSOR_ADAPTOR_H_
#define TAKRAM_TWEEN_ACCESSOR_ADAPTOR_H_

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

#include "takram/easing.h"
#include "takram/tween/adaptor_base.h"
#include "takram/tween/hash.h"
#include "takram/tween/transform.h"

namespace takram {
namespace tween {

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
class AccessorAdaptor : public AdaptorBase<Interval> {
 public:
  using Value = T;

  // Constructors
  AccessorAdaptor(Class *target,
                  Getter getter,
                  Setter setter,
                  const std::string& name,
                  const Value& to,
                  const Easing& easing,
                  const Interval& duration,
                  const Interval& delay,
                  const std::function<void()>& callback);

  // Disallow copy and assign
  AccessorAdaptor(const AccessorAdaptor&) = delete;
  AccessorAdaptor& operator=(const AccessorAdaptor&) = delete;

  // Hash
  std::size_t key() const override;
  std::size_t hash() const override;

 protected:
  // Updates against the local unit time
  void update(double unit) override;

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
inline AccessorAdaptor<Interval, T, Class, Getter, Setter>
    ::AccessorAdaptor(Class *target,
                      Getter getter,
                      Setter setter,
                      const std::string& name,
                      const Value& to,
                      const Easing& easing,
                      const Interval& duration,
                      const Interval& delay,
                      const std::function<void()>& callback)
    : AdaptorBase<Interval>(easing, duration, delay, callback),
      target_(target),
      getter_(getter),
      setter_(setter),
      hash_(std::hash<std::string>()(name)),
      from_((target->*getter)()),
      to_(to) {}

#pragma mark Updates against the local unit time

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline void AccessorAdaptor<Interval, T, Class, Getter, Setter>
    ::update(double unit) {
  if (unit < 0.0) {
    from_ = (target_->*getter_)();
  } else if (AdaptorBase<Interval>::duration_.empty() || unit > 1.0) {
    (target_->*setter_)(Transform(this->easing_, 1.0, from_, to_));
  } else {
    (target_->*setter_)(Transform(this->easing_, unit, from_, to_));
  }
}

#pragma mark Hash

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline std::size_t AccessorAdaptor<Interval, T, Class, Getter, Setter>
    ::key() const {
  return Hash(target_);
}

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline std::size_t AccessorAdaptor<Interval, T, Class, Getter, Setter>
    ::hash() const {
  return hash_;
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_ACCESSOR_ADAPTOR_H_
