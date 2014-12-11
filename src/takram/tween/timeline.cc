//
//  takram/tween/timeline.cc
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

#include "takram/tween/timeline.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "takram/tween/interval.h"

namespace takram {
namespace tween {

#pragma mark Managing tweens

template <typename Interval>
void Timeline<Interval>::add(Adapter adapter, bool overwrite) {
  assert(adapter);
  const auto key = adapter->key();
  Hashes *hashes = nullptr;
  if (keys_.find(key) == keys_.end()) {
    hashes = keys_.emplace(key, std::make_unique<Hashes>()).first->second.get();
  } else {
    hashes = keys_.at(key).get();
  }
  assert(hashes);
  const auto hash = adapter->hash();
  if (overwrite && keys_.find(key) != keys_.end()) {
    hashes->erase(hash);
  }
  hashes->emplace(hash, adapter);
}

template <typename Interval>
void Timeline<Interval>::remove(Adapter adapter) {
  assert(adapter);
  const auto key = adapter->key();
  if (keys_.find(key) != keys_.end()) {
    const auto hashes = keys_.at(key).get();
    assert(hashes);
    hashes->erase(adapter->hash());
  }
}

template <typename Interval>
bool Timeline<Interval>::contains(Adapter adapter) const {
  assert(adapter);
  const auto key = adapter->key();
  if (keys_.find(key) != keys_.end()) {
    const auto keys = keys_.at(key).get();
    assert(keys);
    return keys->find(adapter->hash()) != keys->end();
  }
  return false;
}

#pragma mark Advances the timeline

template <typename Interval>
Interval Timeline<Interval>::advance() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  const auto now = clock_.advance();
  // Intentionally copy the key store because subsequent process will change
  // the contents of the key store.
  std::unordered_map<std::size_t, Hashes *> keys;
  using pair = typename decltype(keys_)::value_type;
  std::for_each(keys_.begin(), keys_.end(), [&keys](pair& pair) {
    keys.emplace(pair.first, pair.second.get());
  });
  for (const auto& key_hashes_pair : keys) {
    assert(key_hashes_pair.second);
    // Intentionally copy the hash store here because update call of the
    // adapters can change contents of the hash store.
    const auto hashes = *key_hashes_pair.second;
    for (const auto& hash_adapter_pair : hashes) {
      auto adapter = hash_adapter_pair.second;
      assert(adapter);
      if (adapter->running()) {
        adapter->update(now);
      }
    }
  }
  for (const auto& key_hashes_pair : keys) {
    const auto hashes = key_hashes_pair.second;
    assert(hashes);
    for (auto itr = hashes->begin(); itr != hashes->end();) {
      const auto adapter = itr->second;
      assert(adapter);
      if (adapter->finished()) {
        hashes->erase(itr++);
      } else {
        ++itr;
      }
    }
  }
  for (auto itr = keys_.begin(); itr != keys_.end();) {
    const auto hashes = itr->second.get();
    assert(hashes);
    if (hashes->empty()) {
      keys_.erase(itr++);
    } else {
      ++itr;
    }
  }
  return now;
}

template class Timeline<Time>;
template class Timeline<Frame>;

}  // namespace tween
}  // namespace takram
