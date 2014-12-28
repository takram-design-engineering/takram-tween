//
//  test/tween/accessor_adaptor_test.cc
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

#include <thread>
#include <utility>

#include "gtest/gtest.h"

#include "takram/easing.h"
#include "takram/tween/clock.h"
#include "takram/tween/interval.h"
#include "takram/tween/accessor_adaptor.h"

namespace takram {
namespace tween {

template <typename T>
class AccessorAdaptorTest : public ::testing::Test {};

using Types = ::testing::Types<
  Time,
  Frame
>;
TYPED_TEST_CASE(AccessorAdaptorTest, Types);

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

TEST(AccessorAdaptorTest, Test) {
  Type value;
  AccessorAdaptor<
      Time, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)> adaptor(
          &value, &Type::value, &Type::set_value, "value", 1.0,
          LinearEasing::In, Time(1.0), Time(0.0), []() {});
}

TYPED_TEST(AccessorAdaptorTest, Constructible) {
  Type from1;
  Type from2;
  Type to1;
  Type to2;
  const auto adaptor1 = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &from1, &Type::value, &Type::set_value, "value", to1.value(),
          LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  const auto adaptor2 = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &from2, &Type::value, &Type::set_value, "value", to2.value(),
          LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  ASSERT_EQ(adaptor1.object(), &from1);
  ASSERT_EQ(adaptor2.object(), &from2);
  ASSERT_EQ(adaptor1.from(), from1.value());
  ASSERT_EQ(adaptor2.from(), from2.value());
  ASSERT_EQ(adaptor1.to(), to1.value());
  ASSERT_EQ(adaptor2.to(), to2.value());
}

TYPED_TEST(AccessorAdaptorTest, MoveConstructible) {
  Type from;
  Type to;
  auto adaptor1 = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &from, &Type::value, &Type::set_value, "value", to.value(),
          LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  const auto adaptor2 = std::move(adaptor1);
  ASSERT_EQ(adaptor2.object(), &from);
  ASSERT_EQ(adaptor2.from(), from.value());
  ASSERT_EQ(adaptor2.to(), to.value());
}

TYPED_TEST(AccessorAdaptorTest, Hash) {
  Type value1;
  const auto adaptor1 = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value1, &Type::value, &Type::set_value, "value", value1.value(),
          LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  Type value2;
  const auto adaptor2 = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value2, &Type::value, &Type::set_value, "value", value2.value(),
          LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  const auto adaptor3 = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value2, &Type::value, &Type::set_value, "value", value2.value(),
          LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  const auto adaptor4 = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value2, &Type::value, &Type::set_value, "other", value2.value(),
          LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  ASSERT_NE(adaptor1.object_hash(), adaptor2.object_hash());
  ASSERT_EQ(adaptor1.target_hash(), adaptor2.target_hash());
  ASSERT_EQ(adaptor2.object_hash(), adaptor3.object_hash());
  ASSERT_EQ(adaptor2.target_hash(), adaptor3.target_hash());
  ASSERT_EQ(adaptor3.object_hash(), adaptor4.object_hash());
  ASSERT_NE(adaptor3.target_hash(), adaptor4.target_hash());
}

TYPED_TEST(AccessorAdaptorTest, Control) {
  Clock<TypeParam> clock;
  Type value;
  auto adaptor = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value, &Type::value, &Type::set_value, "value", value.value(),
          LinearEasing::In, TypeParam(), TypeParam(), nullptr);
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

TYPED_TEST(AccessorAdaptorTest, Callback) {
  bool callbacked = false;
  auto callback = [&callbacked]() {
    callbacked = true;
  };
  Clock<TypeParam> clock;
  Type value;
  auto adaptor = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value, &Type::value, &Type::set_value, "value", value.value(),
          LinearEasing::In, TypeParam(), TypeParam(), callback);
  adaptor.start(clock.now());
  ASSERT_EQ(callbacked, false);
  adaptor.AdaptorBase<TypeParam>::update(clock.advance());
  ASSERT_EQ(callbacked, true);
}

TYPED_TEST(AccessorAdaptorTest, CallbackSuppression) {
  bool callbacked = false;
  auto callback = [&callbacked]() {
    callbacked = true;
  };
  Clock<TypeParam> clock;
  Type value;
  auto adaptor = AccessorAdaptor<
      TypeParam, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value, &Type::value, &Type::set_value, "value", value.value(),
          LinearEasing::In, TypeParam(), TypeParam(), callback);
  adaptor.start(clock.now());
  ASSERT_EQ(callbacked, false);
  adaptor.AdaptorBase<TypeParam>::update(clock.advance(), false);
  ASSERT_EQ(callbacked, false);
}

TEST(AccessorAdaptorTest, TimeDuration) {
  Clock<Time> clock;
  const Type from(0.0);
  const Type to(1.0);
  Type value = from;
  auto adaptor = AccessorAdaptor<
      Time, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value, &Type::value, &Type::set_value, "value", to.value(),
          LinearEasing::In, Time(0.1), Time(), nullptr);
  adaptor.start(clock.now());
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_NE(value.value(), from.value());
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_EQ(value.value(), to.value());
  ASSERT_EQ(adaptor.running(), false);
  ASSERT_EQ(adaptor.finished(), true);
}

TEST(AccessorAdaptorTest, FrameDuration) {
  Clock<Frame> clock;
  const Type from(0.0);
  const Type to(1.0);
  Type value = from;
  auto adaptor = AccessorAdaptor<
      Frame, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value, &Type::value, &Type::set_value, "value", to.value(),
          LinearEasing::In, Frame(10), Frame(), nullptr);
  adaptor.start(clock.now());
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value.value(), 0.5);
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value.value(), to.value());
  ASSERT_EQ(adaptor.running(), false);
  ASSERT_EQ(adaptor.finished(), true);
}

TEST(AccessorAdaptorTest, TimeDelay) {
  Clock<Time> clock;
  const Type from(0.0);
  const Type to(1.0);
  Type value = from;
  auto adaptor = AccessorAdaptor<
      Time, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value, &Type::value, &Type::set_value, "value", to.value(),
          LinearEasing::In, Time(0.1), Time(0.1), nullptr);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_EQ(value.value(), from.value());
  adaptor.start(clock.now());
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_EQ(value.value(), from.value());
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  adaptor.AdaptorBase<Time>::update(clock.advance());
  ASSERT_NE(value.value(), from.value());
}

TEST(AccessorAdaptorTest, FrameDelay) {
  Clock<Frame> clock;
  const Type from(0.0);
  const Type to(1.0);
  Type value = from;
  auto adaptor = AccessorAdaptor<
      Frame, double,
      Type, decltype(&Type::value), decltype(&Type::set_value)>(
          &value, &Type::value, &Type::set_value, "value", to.value(),
          LinearEasing::In, Frame(10), Frame(10), nullptr);
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value.value(), from.value());
  adaptor.start(clock.now());
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value.value(), from.value());
  clock.advance(); clock.advance(); clock.advance(); clock.advance();
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_EQ(value.value(), from.value());
  adaptor.AdaptorBase<Frame>::update(clock.advance());
  ASSERT_NE(value.value(), from.value());
}

}  // namespace tween
}  // namespace takram
