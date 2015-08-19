//
//  test/tween/interval_test.cc
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

#include <chrono>
#include <thread>

#include "gtest/gtest.h"

#include "takram/tween/clock.h"

namespace takram {
namespace tween {

TEST(ClockTest, DefaultConstructible) {
  Clock<Time> time_clock;
  Clock<Frame> frame_clock;
  ASSERT_NE(time_clock.now(), Time());
  ASSERT_NE(frame_clock.now(), Frame());
}

TEST(ClockTest, CopyConstructible) {
  Clock<Time> time_clock1;
  Clock<Frame> frame_clock1;
  Clock<Time> time_clock2(time_clock1);
  Clock<Frame> frame_clock2(frame_clock1);
  ASSERT_EQ(time_clock1, time_clock2);
  ASSERT_EQ(frame_clock1, frame_clock2);
}

TEST(ClockTest, Assignable) {
  Clock<Time> time_clock1;
  Clock<Frame> frame_clock1;
  Clock<Time> time_clock2;
  Clock<Frame> frame_clock2;
  time_clock2 = time_clock1;
  frame_clock2 = frame_clock1;
  ASSERT_EQ(time_clock1, time_clock2);
  ASSERT_EQ(frame_clock1, frame_clock2);
}

TEST(ClockTest, Advance) {
  Clock<Time> time_clock;
  auto time = time_clock.now();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  time_clock.advance();
  ASSERT_GE((time_clock.now() - time).count(), 0.5);
  ASSERT_NEAR((time_clock.now() - time).count(), 0.5, 1.0 / 60.0);
  Clock<Frame> frame_clock;
  auto frame = frame_clock.now();
  frame_clock.advance();
  ASSERT_EQ((frame_clock.now() - frame).count(), 1);
}

}  // namespace tween
}  // namespace takram
