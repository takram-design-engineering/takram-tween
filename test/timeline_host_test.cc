//
//  timeline_host_test.cc
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

#include "gtest/gtest.h"

#include "takram/easing.h"
#include "takram/tween/interval.h"
#include "takram/tween/timeline_host.h"
#include "takram/tween/tween.h"

namespace takram {
namespace tween {

namespace {

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

template <class T>
class TimelineHostTest : public ::testing::Test {};

using Intervals = ::testing::Types<Time, Frame>;
TYPED_TEST_CASE(TimelineHostTest, Intervals);

TYPED_TEST(TimelineHostTest, TweenWithPointer) {
  double value{};
  typename TypeParam::Value interval{};
  const auto easing = LinearEasing::In;
  const auto callback = []() {};
  TimelineHost host;
  Tween<TypeParam> tween;
  tween = host.tween<TypeParam>(&value, value, easing, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  tween = host.tween<TypeParam>(&value, value, easing, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  tween = host.tween<TypeParam>(&value, value, easing, interval, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  tween = host.tween<TypeParam>(&value, value, easing, interval, interval,
                                callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
}

TYPED_TEST(TimelineHostTest, TweenWithAccessor) {
  Type value{};
  typename TypeParam::Value interval{};
  const auto easing = LinearEasing::In;
  const auto callback = []() {};
  TimelineHost host;
  Tween<TypeParam> tween;
  tween = host.tween<TypeParam>(&value, &Type::value, &Type::set_value,
                                "value", value.value(),
                                easing, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  tween = host.tween<TypeParam>(&value, &Type::value, &Type::set_value,
                                "value", value.value(),
                                easing, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  tween = host.tween<TypeParam>(&value, &Type::value, &Type::set_value,
                                "value", value.value(),
                                easing, interval, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  tween = host.tween<TypeParam>(&value, &Type::value, &Type::set_value,
                                "value", value.value(),
                                easing, interval, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
}

}  // namespace tween
}  // namespace takram
