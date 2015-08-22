//
//  takram/tween/timeline.cc
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

#pragma mark Managing adaptors

template <class Interval>
void Timeline<Interval>::add(Adaptor adaptor, bool overwrite) {
  assert(adaptor);
  const auto object = adaptor->object_hash();
  const auto target = adaptor->target_hash();
  std::lock_guard<std::recursive_mutex> lock(*mutex_);
  if (objects_.find(object) == objects_.end()) {
    auto& targets = objects_.emplace(object, Targets()).first->second;
    targets.emplace(target, adaptor);
  } else {
    auto& targets = objects_.at(object);
    if (overwrite) {
      targets.erase(target);
    }
    targets.emplace(target, adaptor);
  }
}

template <class Interval>
void Timeline<Interval>::remove(Adaptor adaptor) {
  assert(adaptor);
  const auto object = adaptor->object_hash();
  std::lock_guard<std::recursive_mutex> lock(*mutex_);
  if (objects_.find(object) != objects_.end()) {
    auto& targets = objects_.at(object);
    const auto itr = std::find_if(
        targets.begin(), targets.end(),
        [&adaptor](const std::pair<std::size_t, Adaptor>& pair) {
          return pair.second == adaptor;
        });
    if (itr != targets.end()) {
      targets.erase(itr);
      if (targets.empty()) {
        objects_.erase(object);
      }
    }
  }
}

template <class Interval>
bool Timeline<Interval>::contains(Adaptor adaptor) const {
  assert(adaptor);
  const auto object = adaptor->object_hash();
  std::lock_guard<std::recursive_mutex> lock(*mutex_);
  if (objects_.find(object) != objects_.end()) {
    const auto& targets = objects_.at(object);
    const auto itr = std::find_if(
        targets.begin(), targets.end(),
        [&adaptor](const std::pair<std::size_t, Adaptor>& pair) {
          return pair.second == adaptor;
        });
    return itr != targets.end();
  }
  return false;
}

template <class Interval>
void Timeline<Interval>::clear() {
  std::lock_guard<std::recursive_mutex> lock(*mutex_);
  return objects_.clear();
}

template <class Interval>
bool Timeline<Interval>::empty() const {
  std::lock_guard<std::recursive_mutex> lock(*mutex_);
  return objects_.empty();
}

#pragma mark Advances the timeline

template <class Interval>
Interval Timeline<Interval>::advance() {
  std::vector<Adaptor> finished_adaptors;
  std::unique_lock<std::recursive_mutex> lock(*mutex_);
  const auto now = clock_.advance();
  for (auto object_itr = objects_.begin(); object_itr != objects_.end();) {
    auto& targets = object_itr->second;
    for (auto target_itr = targets.begin(); target_itr != targets.end();) {
      const auto adaptor = target_itr->second;
      assert(adaptor);
      if (adaptor->running()) {
        adaptor->update(now, false);
      }
      if (adaptor->finished()) {
        targets.erase(target_itr++);
        finished_adaptors.emplace_back(adaptor);
      } else {
        ++target_itr;
      }
    }
    if (targets.empty()) {
      objects_.erase(object_itr++);
    } else {
      ++object_itr;
    }
  }
  lock.unlock();
  for (auto& adaptor : finished_adaptors) {
    if (adaptor->callback()) {
      adaptor->callback()();
    }
  }
  return now;
}

template class Timeline<Time>;
template class Timeline<Frame>;

}  // namespace tween
}  // namespace takram
