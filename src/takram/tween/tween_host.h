//
//  takram/tween/tween_host.h
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
#ifndef TAKRAM_TWEEN_TWEEN_HOST_H_
#define TAKRAM_TWEEN_TWEEN_HOST_H_

#include "takram/tween/tween_host_base.h"
#include "takram/tween/tween_runner.h"

namespace takram {

class TweenHost : public TweenHostBase {
 public:
  // Constructors
  TweenHost();

  // Accessing tween runner
  TweenRunner& tween_runner() { return tween_runner_; }
  const TweenRunner& tween_runner() const { return tween_runner_; }

 private:
  // Data members
  TweenRunner tween_runner_;
};

#pragma mark - Inline Implementations

inline TweenHost::TweenHost()
    : TweenHostBase() {
  set_tween_runner(&tween_runner_);
}

}  // namespace takram

#endif  // TAKRAM_TWEEN_TWEEN_HOST_H_
