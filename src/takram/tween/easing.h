//
//  takram/tween/easing.h
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
#ifndef TAKRAM_TWEEN_EASING_H_
#define TAKRAM_TWEEN_EASING_H_

#include "takram/easing/preset.h"
#include "takram/easing/types.h"
#include "takram/tween/types.h"

namespace takram {
namespace tween {

using Easing = easing::Easing<Unit>;
using BackEasing = easing::BackEasing<Unit>;
using BounceEasing = easing::BounceEasing<Unit>;
using CircularEasing = easing::CircularEasing<Unit>;
using CubicEasing = easing::CubicEasing<Unit>;
using ElasticEasing = easing::ElasticEasing<Unit>;
using ExponentialEasing = easing::ExponentialEasing<Unit>;
using LinearEasing = easing::LinearEasing<Unit>;
using QuadraticEasing = easing::QuadraticEasing<Unit>;
using QuarticEasing = easing::QuarticEasing<Unit>;
using QuinticEasing = easing::QuinticEasing<Unit>;
using SinusoidalEasing = easing::SinusoidalEasing<Unit>;

}  // namespace tween
}  // namespace takram

#endif  // TAKRAM_TWEEN_EASING_H_

