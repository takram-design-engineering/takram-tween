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

#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "takram/tween/adaptor.h"
#include "takram/tween/clock.h"
#include "takram/tween/hash.h"

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
  std::unordered_map<std::size_t, Targets> objects_;
  Clock<Interval> clock_;
  std::unique_ptr<std::recursive_mutex> mutex_;
};

#pragma mark -

template <class Interval>
inline Timeline<Interval>::Timeline()
    : mutex_(std::make_unique<std::recursive_mutex>()) {}

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
