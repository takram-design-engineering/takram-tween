//
//  takram/tween/types.h
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
#ifndef TAKRAM_TWEEN_TYPES_H_
#define TAKRAM_TWEEN_TYPES_H_

#include <cstdint>
#include <functional>

#ifndef TAKRAM_TWEEN_UNIT_TYPE
#define TAKRAM_TWEEN_UNIT_TYPE double
#endif

#ifndef TAKRAM_TWEEN_TIME_VALUE_TYPE
#define TAKRAM_TWEEN_TIME_VALUE_TYPE double
#endif

#ifndef TAKRAM_TWEEN_FRAME_VALUE_TYPE
#define TAKRAM_TWEEN_FRAME_VALUE_TYPE std::int64_t
#endif

namespace takram {
namespace tween {

using Unit = TAKRAM_TWEEN_UNIT_TYPE;
using TimeValue = TAKRAM_TWEEN_TIME_VALUE_TYPE;
using FrameValue = TAKRAM_TWEEN_FRAME_VALUE_TYPE;
using Callback = std::function<void()>;

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_TYPES_H_

