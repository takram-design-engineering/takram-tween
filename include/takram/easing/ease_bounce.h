//
//  takram/easing/ease_bounce.h
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

#pragma once
#ifndef TAKRAM_EASING_EASE_BOUNCE_H_
#define TAKRAM_EASING_EASE_BOUNCE_H_

namespace takram {
namespace easing {

inline double EaseBounce(double t) {
  const double v = 1.0 - t;
  double c;
  double d;
  if (v < (1.0 / 2.750)) {
    c = v;
    d = 0;
  } else if (v < (2.0 / 2.750)) {
    c = v - 1.50 / 2.750;
    d = 0.750;
  } else if (v < (2.5 / 2.750)) {
    c = v - 2.250 / 2.750;
    d = 0.93750;
  } else {
    c = v - 2.6250 / 2.750;
    d = 0.9843750;
  }
  return 1.0 - (7.56250 * c * c + d);
}

}  // namespace easing
}  // namespace takram

#endif  // TAKRAM_EASING_EASE_BOUNCE_H_
