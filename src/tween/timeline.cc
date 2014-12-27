//
//  takram/tween/timeline.cc
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

#include "takram/tween/timeline.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "takram/tween/interval.h"

namespace takram {
namespace tween {

#pragma mark Managing tweens

template <typename Interval>
void Timeline<Interval>::add(Adaptor adaptor, bool overwrite) {
  assert(adaptor);
  const auto key = adaptor->key();
  const auto hash = adaptor->hash();
  std::lock_guard<std::mutex> lock(mutex_);
  if (keys_.find(key) == keys_.end()) {
    auto& hashes = keys_.emplace(key, Hashes()).first->second;
    hashes.emplace(hash, adaptor);
  } else {
    auto& hashes = keys_.at(key);
    if (overwrite && keys_.find(key) != keys_.end()) {
      hashes.erase(hash);
    }
    hashes.emplace(hash, adaptor);
  }
}

template <typename Interval>
void Timeline<Interval>::remove(Adaptor adaptor) {
  assert(adaptor);
  const auto key = adaptor->key();
  std::lock_guard<std::mutex> lock(mutex_);
  if (keys_.find(key) != keys_.end()) {
    auto& hashes = keys_.at(key);
    const auto itr = std::find_if(
        hashes.begin(), hashes.end(),
        [&adaptor](const std::pair<std::size_t, Adaptor>& pair) {
          return pair.second == adaptor;
        });
    if (itr != hashes.end()) {
      hashes.erase(itr);
      if (hashes.empty()) {
        keys_.erase(key);
      }
    }
  }
}

template <typename Interval>
bool Timeline<Interval>::contains(Adaptor adaptor) const {
  assert(adaptor);
  const auto key = adaptor->key();
  std::lock_guard<std::mutex> lock(mutex_);
  if (keys_.find(key) != keys_.end()) {
    const auto& hashes = keys_.at(key);
    const auto itr = std::find_if(
        hashes.begin(), hashes.end(),
        [&adaptor](const std::pair<std::size_t, Adaptor>& pair) {
          return pair.second == adaptor;
        });
    return itr != hashes.end();
  }
  return false;
}

template <typename Interval>
bool Timeline<Interval>::empty() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return keys_.empty();
}

#pragma mark Advances the timeline

template <typename Interval>
Interval Timeline<Interval>::advance() {
  std::vector<Adaptor> finished_adaptors;
  mutex_.lock();
  const auto now = clock_.advance();
  for (auto keys_itr = keys_.begin(); keys_itr != keys_.end();) {
    auto& hashes = keys_itr->second;
    for (auto hashes_itr = hashes.begin(); hashes_itr != hashes.end();) {
      const auto adaptor = hashes_itr->second;
      assert(adaptor);
      if (adaptor->running()) {
        adaptor->update(now, false);
      }
      if (adaptor->finished()) {
        hashes.erase(hashes_itr++);
        finished_adaptors.emplace_back(adaptor);
      } else {
        ++hashes_itr;
      }
    }
    if (hashes.empty()) {
      keys_.erase(keys_itr++);
    } else {
      ++keys_itr;
    }
  }
  mutex_.unlock();
  for (auto& adaptor : finished_adaptors) {
    adaptor->callback()();
  }
  return now;
}

template class Timeline<Time>;
template class Timeline<Frame>;

}  // namespace tween
}  // namespace takram
