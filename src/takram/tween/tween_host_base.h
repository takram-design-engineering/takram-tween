//
//  takram/tween/tween_host_base.h
//
//  takram design engineering Confidential
//
//  Copyright (C) 2014 Shota Matsuda
//
//  All information contained herein is, and remains the property of takram
//  design engineering and its suppliers, if any. The intellectual and
//  technical concepts contained herein are proprietary to takram design
//  engineering and its suppliers and may be covered by U.S. and Foreign
//  Patents, patents in process, and are protected by trade secret or copyright
//  law. Dissemination of this information or reproduction of this material is
//  strictly forbidden unless prior written permission is obtained from takram
//  design engineering.
//

// For contributors: Adhere to Google's C++ coding style, except in case where
// your find rational conventions to retain readability, consistency and
// maintenability otherwise.
// Run cpplint to check your style errors before making a pull request.

#pragma once
#ifndef TAKRAM_TWEEN_TWEEN_HOST_BASE_H_
#define TAKRAM_TWEEN_TWEEN_HOST_BASE_H_

#include <glog/logging.h>

#include <cassert>

#include "takram/tween/easing.h"
#include "takram/tween/tween.h"
#include "takram/tween/tween_runner.h"

namespace takram {

class TweenHostBase {
 public:
  // Constructors
  TweenHostBase();
  explicit TweenHostBase(TweenRunner *runner);
  virtual ~TweenHostBase() = 0;

  // Managing tweens
  template <EasingFunction Easing, typename... Args>
  Tween<Easing> addTween(Args&&... args);  // NOLINT(build/c++11)

  // Managing tween runner
  TweenRunner * tween_runner() { return tween_runner_; }
  const TweenRunner * tween_runner() const { return tween_runner_; }
  void set_tween_runner(TweenRunner *value) { tween_runner_ = value; }

 private:
  // Data members
  TweenRunner *tween_runner_;
};

#pragma mark - Inline Implementations

inline TweenHostBase::TweenHostBase()
    : tween_runner_(nullptr) {}

inline TweenHostBase::TweenHostBase(TweenRunner *runner)
    : tween_runner_(runner) {}

inline TweenHostBase::~TweenHostBase() {}

template <EasingFunction Easing, typename... Args>
inline Tween<Easing> TweenHostBase::addTween(
    Args&&... args) {  // NOLINT(build/c++11)
  if (!tween_runner_) {
    LOG(WARNING) << "Aborting adding tween since runner is not specified.";
  }
  auto tween = Tween<Easing>(args..., tween_runner_);
  tween.start();
  return tween;
}

}  // namespace takram

#endif  // TAKRAM_TWEEN_TWEEN_HOST_BASE_H_
