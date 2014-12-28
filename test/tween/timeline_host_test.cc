//
//  test/tween/timeline_host_test.cc
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

#include "gtest/gtest.h"

#include "takram/easing.h"
#include "takram/tween/timeline_host.h"

namespace takram {
namespace tween {

namespace {

class Host : public TimelineHost<> {
 public:
  Timeline& timeline() override { return timeline_; }
  const Timeline& timeline() const override { return timeline_; }

 private:
  Timeline timeline_;
};

class Type {
 public:
  Type() = default;
  explicit Type(double value) : value_(value) {}
  double value() const { return value_; }
  void set_value(double value) { value_ = value; }

 private:
  double value_;
};

}  // namespace

TEST(TimelineHostTest, TweenWithPointer) {
  double value;
  Host::Interval::Value interval;
  const auto easing = LinearEasing::In;
  const auto callback = []() {};
  Host host;
  Host::Tween tween;
  tween = host.tween(&value, value, easing, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), Host::Interval::Value());
  ASSERT_EQ(tween.timeline(), &host.timeline());
  tween = host.tween(&value, value, easing, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), Host::Interval::Value());
  ASSERT_EQ(tween.timeline(), &host.timeline());
  tween = host.tween(&value, value, easing, interval, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), &host.timeline());
  tween = host.tween(&value, value, easing, interval, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), &host.timeline());
}

TEST(TimelineHostTest, TweenWithAccessor) {
  Type value;
  Host::Interval::Value interval;
  const auto easing = LinearEasing::In;
  const auto callback = []() {};
  Host host;
  Host::Tween tween;
  tween = host.tween(&value, &Type::value, &Type::set_value,
                     "value", value.value(),
                     easing, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), Host::Interval::Value());
  ASSERT_EQ(tween.timeline(), &host.timeline());
  tween = host.tween(&value, &Type::value, &Type::set_value,
                     "value", value.value(),
                     easing, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), Host::Interval::Value());
  ASSERT_EQ(tween.timeline(), &host.timeline());
  tween = host.tween(&value, &Type::value, &Type::set_value,
                     "value", value.value(),
                     easing, interval, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), &host.timeline());
  tween = host.tween(&value, &Type::value, &Type::set_value,
                     "value", value.value(),
                     easing, interval, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), &host.timeline());
}

}  // namespace tween
}  // namespace takram
