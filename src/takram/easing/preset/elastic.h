//
//  takram/easing/preset/elastic.h
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

#pragma once
#ifndef TAKRAM_EASING_PRESET_ELASTIC_H_
#define TAKRAM_EASING_PRESET_ELASTIC_H_

#include <cmath>

#include "takram/easing/group.h"
#include "takram/math/constants.h"

namespace takram {
namespace easing {
namespace preset {

template <class T>
struct Elastic {
  T operator()(T parameter) {
    const T v = parameter - 1.0;
    const T p = 0.3;
    return -std::pow(2.0, 10.0 * v) *
        std::sin((v - p / 4.0) * math::tau<T> / p);
  }
};

template <class T>
using ElasticEasing = Group<T, Elastic>;

}  // namespace preset

using preset::ElasticEasing;

}  // namespace easing
}  // namespace takram

#endif  // TAKRAM_EASING_PRESET_ELASTIC_H_
