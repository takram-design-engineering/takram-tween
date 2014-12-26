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

#include "gtest/gtest.h"

#include "takram/easing.h"
#include "takram/tween/interval.h"
#include "takram/tween/timeline.h"
#include "takram/tween/tween.h"

namespace takram {
namespace tween {

class Type {
 public:
  Type(double value) : value_(value) {}
  double value() { return value_; }
  void set_value(double value) { value_ = value; }

 private:
  double value_;
};

TEST(TweenTest, Time) {
  double value = 1.0;
  Tween<Time> tween;
  tween = Tween<Time>();
  ASSERT_EQ(tween.duration(), 0.0);
  ASSERT_EQ(tween.delay(), 0.0);
  tween = Tween<Time>(&value, 2.0, QuadraticEasing::Out, 3.0);
  ASSERT_EQ(tween.duration(), 3.0);
  ASSERT_EQ(tween.delay(), 0.0);
  tween = Tween<Time>(&value, 2.0, QuadraticEasing::Out, 3.0, []() {});
  ASSERT_EQ(tween.duration(), 3.0);
  ASSERT_EQ(tween.delay(), 0.0);
  tween = Tween<Time>(&value, 2.0, QuadraticEasing::Out, 3.0, 4.0);
  ASSERT_EQ(tween.duration(), 3.0);
  ASSERT_EQ(tween.delay(), 4.0);
  tween = Tween<Time>(&value, 2.0, QuadraticEasing::Out, 3.0, 4.0, []() {});
  ASSERT_EQ(tween.duration(), 3.0);
  ASSERT_EQ(tween.delay(), 4.0);
}

TEST(TweenTest, Frame) {
  Type value = 1.0;
  Timeline<Frame> timeline;
  Tween<Frame> tween;
  ASSERT_EQ(tween.duration(), 0.0);
  ASSERT_EQ(tween.delay(), 0.0);
  tween = Tween<Frame>(&value, &Type::value, &Type::set_value, "value", 2.0,
                       QuadraticEasing::Out, 3.0);
  ASSERT_EQ(tween.duration(), 3.0);
  ASSERT_EQ(tween.delay(), 0.0);
  tween = Tween<Frame>(&value, &Type::value, &Type::set_value, "value", 2.0,
                       QuadraticEasing::Out, 3.0, []() {});
  ASSERT_EQ(tween.duration(), 3.0);
  ASSERT_EQ(tween.delay(), 0.0);
  tween = Tween<Frame>(&value, &Type::value, &Type::set_value, "value", 2.0,
                       QuadraticEasing::Out, 3.0, 4.0);
  ASSERT_EQ(tween.duration(), 3.0);
  ASSERT_EQ(tween.delay(), 4.0);
  tween = Tween<Frame>(&value, &Type::value, &Type::set_value, "value", 2.0,
                       QuadraticEasing::Out, 3.0, 4.0, []() {});
  ASSERT_EQ(tween.duration(), 3.0);
  ASSERT_EQ(tween.delay(), 4.0);
}

}  // namespace tween
}  // namespace takram
