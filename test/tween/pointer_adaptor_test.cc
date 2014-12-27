//
//  test/tween/pointer_adaptor_test.cc
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

#include <thread>
#include <utility>

#include "gtest/gtest.h"

#include "takram/easing.h"
#include "takram/tween/clock.h"
#include "takram/tween/interval.h"
#include "takram/tween/pointer_adaptor.h"

namespace takram {
namespace tween {

template <typename T>
class PointerAdaptorTest : public ::testing::Test {};

using Types = ::testing::Types<
  Time,
  Frame
>;
TYPED_TEST_CASE(PointerAdaptorTest, Types);

TYPED_TEST(PointerAdaptorTest, Constructible) {
  double from1;
  double from2;
  double to1;
  double to2;
  const auto adaptor1 = PointerAdaptor<TypeParam, double>(
      &from1, to1, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  const auto adaptor2 = PointerAdaptor<TypeParam, double>(
      &from2, to2, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  ASSERT_EQ(adaptor1.target(), &from1);
  ASSERT_EQ(adaptor2.target(), &from2);
  ASSERT_EQ(adaptor1.from(), from1);
  ASSERT_EQ(adaptor2.from(), from2);
  ASSERT_EQ(adaptor1.to(), to1);
  ASSERT_EQ(adaptor2.to(), to2);
}

TYPED_TEST(PointerAdaptorTest, MoveConstructible) {
  double from;
  double to;
  auto adaptor1 = PointerAdaptor<TypeParam, double>(
      &from, to, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  const auto adaptor2 = std::move(adaptor1);
  ASSERT_EQ(adaptor2.target(), &from);
  ASSERT_EQ(adaptor2.from(), from);
  ASSERT_EQ(adaptor2.to(), to);
}

TYPED_TEST(PointerAdaptorTest, Hash) {
  double value1;
  const auto adaptor1 = PointerAdaptor<TypeParam, double>(
      &value1, value1, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  double value2;
  const auto adaptor2 = PointerAdaptor<TypeParam, double>(
      &value2, value2, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  const auto adaptor3 = PointerAdaptor<TypeParam, double>(
      &value2, value2, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  ASSERT_NE(adaptor1.key(), adaptor2.key());
  ASSERT_NE(adaptor1.hash(), adaptor2.hash());
  ASSERT_EQ(adaptor2.key(), adaptor3.key());
  ASSERT_EQ(adaptor2.hash(), adaptor3.hash());
}

TYPED_TEST(PointerAdaptorTest, Control) {
  Clock<TypeParam> clock;
  double value;
  auto adaptor = PointerAdaptor<TypeParam, double>(
      &value, value, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  adaptor.start(clock.now());
  ASSERT_EQ(adaptor.running(), true);
  ASSERT_EQ(adaptor.finished(), false);
  adaptor.stop();
  ASSERT_EQ(adaptor.running(), false);
  ASSERT_EQ(adaptor.finished(), false);
  adaptor.start(clock.now());
  ASSERT_EQ(adaptor.running(), true);
  ASSERT_EQ(adaptor.finished(), false);
  adaptor.AdaptorBase<TypeParam>::update(clock.advance());
  ASSERT_EQ(adaptor.running(), false);
  ASSERT_EQ(adaptor.finished(), true);
}

TYPED_TEST(PointerAdaptorTest, Callback) {
  bool callbacked = false;
  auto callback = [&callbacked]() {
    callbacked = true;
  };
  Clock<TypeParam> clock;
  double value;
  auto adaptor = PointerAdaptor<TypeParam, double>(
      &value, value, LinearEasing::In, TypeParam(), TypeParam(), callback);
  adaptor.start(clock.now());
  ASSERT_EQ(callbacked, false);
  adaptor.AdaptorBase<TypeParam>::update(clock.advance());
  ASSERT_EQ(callbacked, true);
}

TYPED_TEST(PointerAdaptorTest, CallbackSuppression) {
  bool callbacked = false;
  auto callback = [&callbacked]() {
    callbacked = true;
  };
  Clock<TypeParam> clock;
  double value;
  auto adaptor = PointerAdaptor<TypeParam, double>(
      &value, value, LinearEasing::In, TypeParam(), TypeParam(), callback);
  adaptor.start(clock.now());
  ASSERT_EQ(callbacked, false);
  adaptor.AdaptorBase<TypeParam>::update(clock.advance(), false);
  ASSERT_EQ(callbacked, false);
}

TEST(PointerAdaptorTest, TimeDuration) {
  Clock<Time> clock;
  const double from = 0.0;
  const double to = 1.0;
  double value = from;
  auto adaptor = PointerAdaptor<Time, double>(
      &value, to, LinearEasing::In, Time(0.1), Time(), nullptr);
  adaptor.start(clock.now());
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_NE(value, from);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_EQ(value, to);
  ASSERT_EQ(adaptor.running(), false);
  ASSERT_EQ(adaptor.finished(), true);
}

TEST(PointerAdaptorTest, FrameDuration) {
  Clock<Frame> clock;
  const double from = 0.0;
  const double to = 1.0;
  double value = from;
  auto adaptor = PointerAdaptor<Frame, double>(
      &value, to, LinearEasing::In, Frame(10), Frame(), nullptr);
  adaptor.start(clock.now());
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value, 0.5);
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value, to);
  ASSERT_EQ(adaptor.running(), false);
  ASSERT_EQ(adaptor.finished(), true);
}

TEST(PointerAdaptorTest, TimeDelay) {
  Clock<Time> clock;
  const double from = 0.0;
  const double to = 1.0;
  double value = from;
  auto adaptor = PointerAdaptor<Time, double>(
      &value, to, LinearEasing::In, Time(0.1), Time(0.1), nullptr);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_EQ(value, from);
  adaptor.start(clock.now());
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_EQ(value, from);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_NE(value, from);
}

TEST(PointerAdaptorTest, FrameDelay) {
  Clock<Frame> clock;
  const double from = 0.0;
  const double to = 1.0;
  double value = from;
  auto adaptor = PointerAdaptor<Frame, double>(
      &value, to, LinearEasing::In, Frame(10), Frame(10), nullptr);
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value, from);
  adaptor.start(clock.now());
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value, from);
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value, from);
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_NE(value, from);
}

}  // namespace tween
}  // namespace takram
