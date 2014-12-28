//
//  takram/tween/accessor_adaptor.h
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
#ifndef TAKRAM_TWEEN_ACCESSOR_ADAPTOR_H_
#define TAKRAM_TWEEN_ACCESSOR_ADAPTOR_H_

#include <cassert>
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

template <typename Interval_, typename T,
          typename Class_, typename Getter_, typename Setter_>
class AccessorAdaptor : public AdaptorBase<Interval_> {
 public:
  using Interval = Interval_;
  using Value = T;
  using Class = Class_;
  using Getter = Getter_;
  using Setter = Setter_;

  // Constructors
  AccessorAdaptor(Class *object,
                  Getter getter,
                  Setter setter,
                  const std::string& name,
                  const Value& to,
                  const Easing& easing,
                  const Interval& duration,
                  const Interval& delay,
                  const std::function<void()>& callback);
  AccessorAdaptor(AccessorAdaptor&& other) = default;

  // Disallow copy and assign
  AccessorAdaptor(const AccessorAdaptor&) = delete;
  AccessorAdaptor& operator=(const AccessorAdaptor&) = delete;

  // Hash
  std::size_t object_hash() const override;
  std::size_t target_hash() const override;

  // Parameters
  Class * object() const { return object_; }
  const Value& from() const { return from_; }
  const Value& to() const { return to_; }

 protected:
  // Updates against the local unit time
  void update(double unit) override;

 private:
  // Data members
  Class *object_;
  Getter getter_;
  Setter setter_;
  std::size_t target_hash_;
  Value from_;
  Value to_;
};

#pragma mark - Inline Implementations

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline AccessorAdaptor<Interval, T, Class, Getter, Setter>
    ::AccessorAdaptor(Class *object,
                      Getter getter,
                      Setter setter,
                      const std::string& name,
                      const Value& to,
                      const Easing& easing,
                      const Interval& duration,
                      const Interval& delay,
                      const std::function<void()>& callback)
    : AdaptorBase<Interval>(easing, duration, delay, callback),
      object_(object),
      getter_(getter),
      setter_(setter),
      target_hash_(std::hash<std::string>()(name)),
      from_((object->*getter)()),
      to_(to) {}

#pragma mark Updates against the local unit time

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline void AccessorAdaptor<Interval, T, Class, Getter, Setter>
    ::update(double unit) {
  assert(object_);
  if (unit < 0.0) {
    from_ = (object_->*getter_)();
  } else if (AdaptorBase<Interval>::duration_.empty() || unit > 1.0) {
    (object_->*setter_)(Transform(this->easing_, 1.0, from_, to_));
  } else {
    (object_->*setter_)(Transform(this->easing_, unit, from_, to_));
  }
}

#pragma mark Hash

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline std::size_t AccessorAdaptor<Interval, T, Class, Getter, Setter>
    ::object_hash() const {
  return Hash(object_);
}

template <typename Interval, typename T,
          typename Class, typename Getter, typename Setter>
inline std::size_t AccessorAdaptor<Interval, T, Class, Getter, Setter>
    ::target_hash() const {
  return target_hash_;
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_ACCESSOR_ADAPTOR_H_
