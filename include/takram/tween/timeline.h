//
//  takram/tween/timeline.h
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

#pragma once
#ifndef TAKRAM_TWEEN_TIMELINE_H_
#define TAKRAM_TWEEN_TIMELINE_H_

#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "takram/tween/adaptor_base.h"
#include "takram/tween/clock.h"
#include "takram/tween/hash.h"

namespace takram {
namespace tween {

template <typename Interval = Time>
class Timeline final {
 public:
  using Adaptor = std::shared_ptr<AdaptorBase<Interval>>;
  using Hashes = std::unordered_map<std::size_t, Adaptor>;

  // Constructors
  Timeline() = default;

  // Disallow copy and assign
  Timeline(const Timeline&) = delete;
  Timeline& operator=(const Timeline&) = delete;

  // Managing tweens
  void add(Adaptor adaptor, bool overwrite = true);
  void remove(Adaptor adaptor);
  bool contains(Adaptor adaptor) const;
  template <typename T>
  void remove(const T *target);
  template <typename T>
  bool contains(const T *target) const;

  // Advances the timeline
  Interval advance();
  Interval now() const { return clock_.now(); }

 private:
  // Data members
  std::unordered_map<std::size_t, std::unique_ptr<Hashes>> keys_;
  Clock<Interval> clock_;
  mutable std::mutex mutex_;
};

#pragma mark - Inline Implementations

#pragma mark Managing tweens

template <typename Interval>
template <typename T>
inline void Timeline<Interval>::remove(const T *target) {
  std::lock_guard<std::mutex> lock(mutex_);
  keys_.erase(Hash(target));
}

template <typename Interval>
template <typename T>
inline bool Timeline<Interval>::contains(const T *target) const {
  std::lock_guard<std::mutex> lock(mutex_);
  return keys_.find(Hash(target)) != keys_.end();
}

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TIMELINE_H_
