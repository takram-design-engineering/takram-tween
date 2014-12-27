//
//  test/tween/timeline_test.cc
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

#include <memory>

#include "gtest/gtest.h"

#include "takram/easing.h"
#include "takram/tween/accessor_adaptor.h"
#include "takram/tween/interval.h"
#include "takram/tween/pointer_adaptor.h"
#include "takram/tween/timeline.h"
#include "takram/tween/tween.h"

namespace takram {
namespace tween {

template <typename T>
class TweenTest : public ::testing::Test {};

using Types = ::testing::Types<
  Time,
  Frame
>;
TYPED_TEST_CASE(TweenTest, Types);

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

TYPED_TEST(TweenTest, DefaultConstructible) {
  Tween<TypeParam> tween;
  ASSERT_EQ(tween, false);
  ASSERT_EQ(tween.empty(), true);
  ASSERT_EQ(tween.timeline(), nullptr);
}

TYPED_TEST(TweenTest, CopyConstructible) {
  double value;
  const auto adaptor = std::make_shared<PointerAdaptor<TypeParam, double>>(
      &value, value, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  Timeline<TypeParam> timeline;
  Tween<TypeParam> tween1(adaptor, &timeline);
  Tween<TypeParam> tween2(tween1);
  ASSERT_TRUE(tween1 == tween2);
}

TYPED_TEST(TweenTest, Assignable) {
  double value;
  const auto adaptor = std::make_shared<PointerAdaptor<TypeParam, double>>(
      &value, value, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  Timeline<TypeParam> timeline;
  Tween<TypeParam> tween1(adaptor, &timeline);
  Tween<TypeParam> tween2;
  tween2 = tween1;
  ASSERT_TRUE(tween1 == tween2);
}

TYPED_TEST(TweenTest, ConstructibleWithPointer) {
  double value;
  typename TypeParam::Value interval;
  const auto easing = LinearEasing::In;
  const auto callback = []() {};
  using Tween = Tween<TypeParam>;
  Tween tween;
  tween = Tween(&value, value, easing, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  ASSERT_EQ(tween.timeline(), nullptr);
  tween = Tween(&value, value, easing, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  ASSERT_EQ(tween.timeline(), nullptr);
  tween = Tween(&value, value, easing, interval, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), nullptr);
  tween = Tween(&value, value, easing, interval, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), nullptr);
  Timeline<TypeParam> timeline;
  tween = Tween(&value, value, easing, interval, &timeline);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  ASSERT_EQ(tween.timeline(), &timeline);
  tween = Tween(&value, value, easing, interval, callback, &timeline);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  ASSERT_EQ(tween.timeline(), &timeline);
  tween = Tween(&value, value, easing, interval, interval, &timeline);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), &timeline);
  tween = Tween(&value, value, easing, interval, interval, callback, &timeline);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), &timeline);
}

TYPED_TEST(TweenTest, ConstructibleWithAccessor) {
  Type value;
  typename TypeParam::Value interval;
  const auto easing = LinearEasing::In;
  const auto callback = []() {};
  using Tween = Tween<TypeParam>;
  Tween tween;
  tween = Tween(&value, &Type::value, &Type::set_value,
                "value", value.value(),
                easing, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  ASSERT_EQ(tween.timeline(), nullptr);
  tween = Tween(&value, &Type::value, &Type::set_value,
                "value", value.value(),
                easing, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  ASSERT_EQ(tween.timeline(), nullptr);
  tween = Tween(&value, &Type::value, &Type::set_value,
                "value", value.value(),
                easing, interval, interval);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), nullptr);
  tween = Tween(&value, &Type::value, &Type::set_value,
                "value", value.value(),
                easing, interval, interval, callback);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), nullptr);
  Timeline<TypeParam> timeline;
  tween = Tween(&value, &Type::value, &Type::set_value,
                "value", value.value(),
                easing, interval, &timeline);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  ASSERT_EQ(tween.timeline(), &timeline);
  tween = Tween(&value, &Type::value, &Type::set_value,
                "value", value.value(),
                easing, interval, callback, &timeline);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), typename TypeParam::Value());
  ASSERT_EQ(tween.timeline(), &timeline);
  tween = Tween(&value, &Type::value, &Type::set_value,
                "value", value.value(),
                easing, interval, interval, &timeline);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), &timeline);
  tween = Tween(&value, &Type::value, &Type::set_value,
                "value", value.value(),
                easing, interval, interval, callback, &timeline);
  ASSERT_EQ(tween.duration(), interval);
  ASSERT_EQ(tween.delay(), interval);
  ASSERT_EQ(tween.timeline(), &timeline);
}

}  // namespace tween
}  // namespace takram
