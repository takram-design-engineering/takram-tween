//
//  takram/tween/transform.cc
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

#include "takram/tween/transform.h"

#include "takram/tween/easing.h"
#include "takram/tween/types.h"

namespace takram {
namespace tween {

template <class T>
T Transform(const Easing& easing, Unit unit, const T& from, const T& to) {
  return from + (to - from) * easing(unit);
}

#define TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(T) \
    template T Transform(const Easing&, Unit, const T&, const T&);

TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(bool)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(char)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(signed char)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(unsigned char)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(char16_t)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(char32_t)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(wchar_t)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(short)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(unsigned short)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(int)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(unsigned int)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(long)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(unsigned long)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(long long)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(unsigned long long)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(float)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(double)
TAKRAM_TWEEN_TRANSFORM_SPECIALIZE(long double)

}  // namespace tween
}  // namespace takram
