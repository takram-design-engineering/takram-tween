//
//  timer_test.cc
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

#include <memory>

#include "gtest/gtest.h"

#include "takram/tween/interval.h"
#include "takram/tween/timeline.h"
#include "takram/tween/timer.h"

namespace takram {
namespace tween {

template <class T>
class TimerTest : public ::testing::Test {};

using Types = ::testing::Types<
  Time,
  Frame
>;
TYPED_TEST_CASE(TimerTest, Types);

TYPED_TEST(TimerTest, DefaultConstructible) {
  Timer<TypeParam> timer;
  ASSERT_EQ(timer, false);
  ASSERT_EQ(timer.empty(), true);
  ASSERT_EQ(timer.timeline(), nullptr);
}

TYPED_TEST(TimerTest, CopyConstructible) {
  double value;
  const auto adaptor = std::make_shared<PointerAdaptor<TypeParam, double>>(
      &value, value, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  Timeline<TypeParam> timeline;
  Timer<TypeParam> timer1(adaptor, &timeline);
  Timer<TypeParam> timer2(timer1);
  ASSERT_TRUE(timer1 == timer2);
}

TYPED_TEST(TimerTest, Assignable) {
  double value;
  const auto adaptor = std::make_shared<PointerAdaptor<TypeParam, double>>(
      &value, value, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  Timeline<TypeParam> timeline;
  Timer<TypeParam> timer1(adaptor, &timeline);
  Timer<TypeParam> timer2;
  timer2 = timer1;
  ASSERT_TRUE(timer1 == timer2);
}

TYPED_TEST(TimerTest, ConstructibleWithPointer) {
  const auto interval = TypeParam();
  const auto callback = []() {};
  using Timer = Timer<TypeParam>;
  Timer timer;
  timer = Timer(interval);
  ASSERT_EQ(timer.duration(), interval);
  ASSERT_EQ(timer.timeline(), nullptr);
  timer = Timer(interval, callback);
  ASSERT_EQ(timer.duration(), interval);
  ASSERT_EQ(timer.timeline(), nullptr);
  Timeline<TypeParam> timeline;
  timer = Timer(interval, &timeline);
  ASSERT_EQ(timer.duration(), interval);
  ASSERT_EQ(timer.timeline(), &timeline);
  timer = Timer(interval, callback, &timeline);
  ASSERT_EQ(timer.duration(), interval);
  ASSERT_EQ(timer.timeline(), &timeline);
}

}  // namespace tween
}  // namespace takram
