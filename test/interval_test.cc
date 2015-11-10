//
//  interval_test.cc
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

#include <cstdint>
#include <limits>

#include "gtest/gtest.h"

#include "takram/math/random.h"
#include "takram/tween/interval.h"

namespace takram {
namespace tween {

template <class T>
class IntervalTest : public ::testing::Test {};

using Types = ::testing::Types<
  std::int16_t,
  std::uint16_t,
  std::int32_t,
  std::uint32_t,
  std::int64_t,
  std::uint64_t,
  float,
  double
>;
TYPED_TEST_CASE(IntervalTest, Types);

TYPED_TEST(IntervalTest, DefaultConstructible) {
  Interval<TypeParam> interval;
  ASSERT_EQ(interval.count(), TypeParam());
}

TYPED_TEST(IntervalTest, ValueConstructible) {
  const auto value = math::Random<>().uniform<TypeParam>();
  Interval<TypeParam> interval(value);
  ASSERT_EQ(interval.count(), value);
}

TYPED_TEST(IntervalTest, CopyConstructible) {
  const auto value = math::Random<>().uniform<TypeParam>();
  Interval<TypeParam> interval1(value);
  Interval<TypeParam> interval2(interval1);
  ASSERT_EQ(interval1, interval2);
}

TYPED_TEST(IntervalTest, Assignable) {
  const auto value = math::Random<>().uniform<TypeParam>();
  Interval<TypeParam> interval1(value);
  Interval<TypeParam> interval2;
  interval2 = interval1;
  ASSERT_EQ(interval1, interval2);
}

TYPED_TEST(IntervalTest, Comparable) {
  const auto min = std::numeric_limits<TypeParam>::min();
  const auto max = std::numeric_limits<TypeParam>::max();
  ASSERT_TRUE(Interval<TypeParam>(min) == Interval<TypeParam>(min));
  ASSERT_TRUE(Interval<TypeParam>(min) != Interval<TypeParam>(max));
  ASSERT_TRUE(Interval<TypeParam>(min) < Interval<TypeParam>(max));
  ASSERT_TRUE(Interval<TypeParam>(max) > Interval<TypeParam>(min));
  ASSERT_TRUE(Interval<TypeParam>(min) <= Interval<TypeParam>(min));
  ASSERT_TRUE(Interval<TypeParam>(min) <= Interval<TypeParam>(max));
  ASSERT_TRUE(Interval<TypeParam>(min) >= Interval<TypeParam>(min));
  ASSERT_TRUE(Interval<TypeParam>(max) >= Interval<TypeParam>(min));
}

TYPED_TEST(IntervalTest, Arithmetic) {
  Interval<TypeParam> interval;
  const auto epsilon = std::numeric_limits<TypeParam>::epsilon();
  interval += Interval<TypeParam>(epsilon);
  ASSERT_EQ(interval.count(), epsilon);
  interval -= Interval<TypeParam>(epsilon);
  ASSERT_EQ(interval.count(), TypeParam());
}

TYPED_TEST(IntervalTest, Count) {
  const auto value = math::Random<>().uniform<TypeParam>();
  Interval<TypeParam> interval(value);
  ASSERT_EQ(interval.count(), value);
}

TYPED_TEST(IntervalTest, Empty) {
  Interval<TypeParam> interval;
  ASSERT_TRUE(interval.empty());
}

}  // namespace tween
}  // namespace takram
