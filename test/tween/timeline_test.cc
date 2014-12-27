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

#include <atomic>
#include <chrono>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "takram/tween/pointer_adaptor.h"
#include "takram/tween/timeline.h"

namespace takram {
namespace tween {

template <typename T>
class TimelineTest : public ::testing::Test {};

using Types = ::testing::Types<
  Time,
  Frame
>;
TYPED_TEST_CASE(TimelineTest, Types);

TYPED_TEST(TimelineTest, DefaultConstructible) {
  Timeline<TypeParam> timeline;
}
  
TYPED_TEST(TimelineTest, AddingAndRemoving) {
  Timeline<TypeParam> timeline;
  double value;
  const auto adaptor1 = std::make_shared<PointerAdaptor<TypeParam, double>>(
      &value, value, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  const auto adaptor2 = std::make_shared<PointerAdaptor<TypeParam, double>>(
      &value, value, LinearEasing::In, TypeParam(), TypeParam(), nullptr);
  timeline.add(adaptor1);
  ASSERT_EQ(timeline.contains(adaptor1), true);
  ASSERT_EQ(timeline.contains(adaptor2), false);
  ASSERT_EQ(timeline.contains(&value), true);
  timeline.remove(&value);
  ASSERT_EQ(timeline.contains(adaptor1), false);
  ASSERT_EQ(timeline.contains(adaptor2), false);
  ASSERT_EQ(timeline.contains(&value), false);
  timeline.add(adaptor1);
  ASSERT_EQ(timeline.contains(adaptor1), true);
  ASSERT_EQ(timeline.contains(adaptor2), false);
  ASSERT_EQ(timeline.contains(&value), true);
  timeline.add(adaptor2);
  ASSERT_EQ(timeline.contains(adaptor1), false);
  ASSERT_EQ(timeline.contains(adaptor2), true);
  ASSERT_EQ(timeline.contains(&value), true);
  timeline.remove(adaptor2);
  ASSERT_EQ(timeline.contains(adaptor1), false);
  ASSERT_EQ(timeline.contains(adaptor2), false);
  ASSERT_EQ(timeline.contains(&value), false);
  timeline.add(adaptor1, false);
  ASSERT_EQ(timeline.contains(adaptor1), true);
  ASSERT_EQ(timeline.contains(adaptor2), false);
  ASSERT_EQ(timeline.contains(&value), true);
  timeline.add(adaptor2, false);
  ASSERT_EQ(timeline.contains(adaptor1), true);
  ASSERT_EQ(timeline.contains(adaptor2), true);
  ASSERT_EQ(timeline.contains(&value), true);
  timeline.remove(adaptor2);
  ASSERT_EQ(timeline.contains(adaptor1), true);
  ASSERT_EQ(timeline.contains(adaptor2), false);
  ASSERT_EQ(timeline.contains(&value), true);
  timeline.remove(adaptor1);
  ASSERT_EQ(timeline.contains(adaptor1), false);
  ASSERT_EQ(timeline.contains(adaptor2), false);
  ASSERT_EQ(timeline.contains(&value), false);
  ASSERT_EQ(timeline.empty(), true);
}

TEST(TimelineTest, TimeAdvance) {
  Timeline<Time> timeline;
  std::mutex mutex;
  std::vector<double *> order;
  double value1 = 0.0;
  double value2 = 0.0;
  double value3 = 0.0;
  auto adaptor1 = std::make_shared<PointerAdaptor<Time, double>>(
      &value1, 1.0, LinearEasing::In, Time(0.2), Time(),
      [&mutex, &order, &value1]() {
        std::lock_guard<std::mutex> lock(mutex);
        order.emplace_back(&value1);
      });
  auto adaptor2 = std::make_shared<PointerAdaptor<Time, double>>(
      &value2, 1.0, LinearEasing::In, Time(0.2), Time(0.1), 
      [&mutex, &order, &value2]() {
        std::lock_guard<std::mutex> lock(mutex);
        order.emplace_back(&value2);
      });
  auto adaptor3 = std::make_shared<PointerAdaptor<Time, double>>(
      &value3, 1.0, LinearEasing::In, Time(0.2), Time(0.05),
      [&mutex, &order, &value3]() {
        std::lock_guard<std::mutex> lock(mutex);
        order.emplace_back(&value3);
      });
  timeline.add(adaptor1);
  timeline.add(adaptor2);
  timeline.add(adaptor3);
  auto future = std::async([&timeline]() {
    while (!timeline.empty()) {
      timeline.advance();
    }
  });
  adaptor1->start(timeline.now());
  adaptor2->start(timeline.now());
  adaptor3->start(timeline.now());
  future.wait();
  ASSERT_EQ(value1, 1.0);
  ASSERT_EQ(value2, 1.0);
  ASSERT_EQ(value3, 1.0);
  ASSERT_EQ(order.size(), 3);
  ASSERT_EQ(order.at(0), &value1);
  ASSERT_EQ(order.at(1), &value3);
  ASSERT_EQ(order.at(2), &value2);
}

TEST(TimelineTest, FrameAdvance) {
  Timeline<Frame> timeline;
  std::mutex mutex;
  std::vector<double *> order;
  double value1 = 0.0;
  double value2 = 0.0;
  double value3 = 0.0;
  auto adaptor1 = std::make_shared<PointerAdaptor<Frame, double>>(
      &value1, 1.0, LinearEasing::In, Frame(200), Frame(),
      [&mutex, &order, &value1]() {
        std::lock_guard<std::mutex> lock(mutex);
        order.emplace_back(&value1);
      });
  auto adaptor2 = std::make_shared<PointerAdaptor<Frame, double>>(
      &value2, 1.0, LinearEasing::In, Frame(200), Frame(100),
      [&mutex, &order, &value2]() {
        std::lock_guard<std::mutex> lock(mutex);
        order.emplace_back(&value2);
      });
  auto adaptor3 = std::make_shared<PointerAdaptor<Frame, double>>(
      &value3, 1.0, LinearEasing::In, Frame(200), Frame(50),
      [&mutex, &order, &value3]() {
        std::lock_guard<std::mutex> lock(mutex);
        order.emplace_back(&value3);
      });
  timeline.add(adaptor1);
  timeline.add(adaptor2);
  timeline.add(adaptor3);
  auto future = std::async([&timeline]() {
    while (!timeline.empty()) {
      timeline.advance();
    }
  });
  adaptor1->start(timeline.now());
  adaptor2->start(timeline.now());
  adaptor3->start(timeline.now());
  future.wait();
  ASSERT_EQ(value1, 1.0);
  ASSERT_EQ(value2, 1.0);
  ASSERT_EQ(value3, 1.0);
  ASSERT_EQ(order.size(), 3);
  ASSERT_EQ(order.at(0), &value1);
  ASSERT_EQ(order.at(1), &value3);
  ASSERT_EQ(order.at(2), &value2);
}

TEST(TimelineTest, RecursiveAdding) {
  Timeline<Time> timeline;
  std::mutex mutex;
  std::vector<double *> order;
  double value1 = 0.0;
  double value2 = 0.0;
  double value3 = 0.0;
  double value4 = 0.0;
  std::shared_ptr<PointerAdaptor<Time, double>> adaptor3;
  std::shared_ptr<PointerAdaptor<Time, double>> adaptor4;
  auto adaptor1 = std::make_shared<PointerAdaptor<Time, double>>(
      &value1, 1.0, LinearEasing::In, Time(0.2), Time(),
      [&timeline, &mutex, &order, &value1, &value3, &adaptor3]() {
        mutex.lock();
        order.emplace_back(&value1);
        mutex.unlock();
        adaptor3 = std::make_shared<PointerAdaptor<Time, double>>(
            &value3, 1.0, LinearEasing::In, Time(0.15), Time(),
            [&mutex, &order, &value3]() {
              std::lock_guard<std::mutex> lock(mutex);
              order.emplace_back(&value3);
            });
        timeline.add(adaptor3);
        adaptor3->start(timeline.now());
      });
  auto adaptor2 = std::make_shared<PointerAdaptor<Time, double>>(
      &value2, 1.0, LinearEasing::In, Time(0.2), Time(0.1), 
      [&timeline, &mutex, &order, &value2, &value4, &adaptor4]() {
        mutex.lock();
        order.emplace_back(&value2);
        mutex.unlock();
        adaptor4 = std::make_shared<PointerAdaptor<Time, double>>(
            &value4, 1.0, LinearEasing::In, Time(0.1), Time(),
            [&mutex, &order, &value4]() {
              std::lock_guard<std::mutex> lock(mutex);
              order.emplace_back(&value4);
            });
        timeline.add(adaptor4);
        adaptor4->start(timeline.now());
      });
  timeline.add(adaptor1);
  timeline.add(adaptor2);
  auto future = std::async([&timeline]() {
    while (!timeline.empty()) {
      timeline.advance();
    }
  });
  adaptor1->start(timeline.now());
  adaptor2->start(timeline.now());
  future.wait();
  ASSERT_EQ(value1, 1.0);
  ASSERT_EQ(value2, 1.0);
  ASSERT_EQ(value3, 1.0);
  ASSERT_EQ(value4, 1.0);
  ASSERT_EQ(order.size(), 4);
  ASSERT_EQ(order.at(0), &value1);
  ASSERT_EQ(order.at(1), &value2);
  ASSERT_EQ(order.at(2), &value3);
  ASSERT_EQ(order.at(3), &value4);
}

}  // namespace tween
}  // namespace takram
