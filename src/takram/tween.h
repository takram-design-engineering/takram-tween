//
//  takram/tween.h
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
#ifndef TAKRAM_TWEEN_H_
#define TAKRAM_TWEEN_H_

namespace takram {
namespace tween {

extern const double version_number;
extern const unsigned char version_string[];

}  // namespace tween
}  // namespace takram

#include "takram/tween/accessor_adaptor.h"
#include "takram/tween/adaptor.h"
#include "takram/tween/clock.h"
#include "takram/tween/easing.h"
#include "takram/tween/hash.h"
#include "takram/tween/interval.h"
#include "takram/tween/pointer_adaptor.h"
#include "takram/tween/timeline.h"
#include "takram/tween/timeline_host.h"
#include "takram/tween/timer_adaptor.h"
#include "takram/tween/transform.h"
#include "takram/tween/types.h"
#include "takram/tween/tween.h"

#endif  // TAKRAM_TWEEN_H_
