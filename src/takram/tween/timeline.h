//
//  takram/tween/timeline.h
//
//  MIT License
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

#pragma once
#ifndef TAKRAM_TWEEN_TIMELINE_H_
#define TAKRAM_TWEEN_TIMELINE_H_

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "takram/tween/adaptor.h"
#include "takram/tween/clock.h"
#include "takram/tween/hash.h"
#include "takram/tween/interval.h"
#include "takram/tween/timer.h"
#include "takram/tween/tween.h"

namespace takram {
namespace tween {

template <class Interval_>
class Timeline final {
 public:
  using Interval = Interval_;

 private:
  using Adaptor = std::shared_ptr<Adaptor<Interval>>;
  using Targets = std::unordered_multimap<std::size_t, Adaptor>;

 public:
  Timeline();

  // Disallow copy semantics
  Timeline(const Timeline& other) = delete;
  Timeline& operator=(const Timeline& other) = delete;

  // Move semantics
  Timeline(Timeline&& other) = default;

  // Shared instance
  static Timeline& shared();

  // Creating tweens
  template <class... Args>
  Tween<Interval> tween(Args&&... args);
  template <class... Args>
  Timer<Interval> timer(Args&&... args);

  // Managing adaptors
  void add(Adaptor adaptor, bool overwrite = true);
  void remove(Adaptor adaptor);
  bool contains(Adaptor adaptor) const;
  template <class T>
  void remove(const T *object);
  template <class T>
  bool contains(const T *object) const;
  void clear();
  bool empty() const;

  // Advances the timeline
  Interval advance();
  Interval now() const { return clock_.now(); }

 private:
  static void delete_shared();

 private:
  std::unordered_map<std::size_t, Targets> objects_;
  Clock<Interval> clock_;
  std::unique_ptr<std::recursive_mutex> mutex_;
  static std::atomic<Timeline *> shared_;
  static std::mutex shared_mutex_;
  static bool shared_deleted_;
};

template <class Interval>
std::atomic<Timeline<Interval> *> Timeline<Interval>::shared_;
template <class Interval>
std::mutex Timeline<Interval>::shared_mutex_;
template <class Interval>
bool Timeline<Interval>::shared_deleted_;

#pragma mark -

template <class Interval>
inline Timeline<Interval>::Timeline()
    : mutex_(std::make_unique<std::recursive_mutex>()) {}

#pragma mark Shared instance

template <class Interval>
inline Timeline<Interval>& Timeline<Interval>::shared() {
  auto shared = shared_.load(std::memory_order_consume);
  if (!shared) {
    std::lock_guard<std::mutex> lock(shared_mutex_);
    shared = shared_.load(std::memory_order_consume);
    if (!shared) {
      assert(!shared_deleted_);
      shared = new Timeline;
      shared_.store(shared, std::memory_order_release);
      std::atexit(&delete_shared);
    }
  }
  return *shared;
}

template <class Interval>
inline void Timeline<Interval>::delete_shared() {
  std::lock_guard<std::mutex> lock(shared_mutex_);
  delete shared_.exchange(nullptr);
  shared_deleted_ = true;
}

#pragma mark Creating tweens

template <class Interval>
template <class... Args>
inline tween::Tween<Interval> Timeline<Interval>::tween(Args&&... args) {
  auto tween = tween::Tween<Interval>(std::forward<Args>(args)..., this);
  tween.start();
  return std::move(tween);
}

template <class Interval>
template <class... Args>
inline tween::Timer<Interval> Timeline<Interval>::timer(Args&&... args) {
  auto timer = tween::Timer<Interval>(std::forward<Args>(args)..., this);
  timer.start();
  return std::move(timer);
}

#pragma mark Managing adaptors

template <class Interval>
template <class T>
inline void Timeline<Interval>::remove(const T *object) {
  std::lock_guard<std::recursive_mutex> lock(*mutex_);
  objects_.erase(Hash(object));
}

template <class Interval>
template <class T>
inline bool Timeline<Interval>::contains(const T *object) const {
  std::lock_guard<std::recursive_mutex> lock(*mutex_);
  return objects_.find(Hash(object)) != objects_.end();
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TIMELINE_H_
