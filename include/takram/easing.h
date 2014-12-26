//
//  takram/easing.h
//
//  MIT License
//
//  Copyright (C) 2014-2015 Shota Matsuda
//  Copyright (C) 2014-2015 takram design engineering
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
#ifndef TAKRAM_EASING_H_
#define TAKRAM_EASING_H_

#include "takram/easing/easing.h"
#include "takram/easing/ease_back.h"
#include "takram/easing/ease_bounce.h"
#include "takram/easing/ease_circular.h"
#include "takram/easing/ease_cubic.h"
#include "takram/easing/ease_elastic.h"
#include "takram/easing/ease_exponential.h"
#include "takram/easing/ease_linear.h"
#include "takram/easing/ease_quadratic.h"
#include "takram/easing/ease_quartic.h"
#include "takram/easing/ease_quintic.h"
#include "takram/easing/ease_sinusoidal.h"

namespace takram {

using easing::Easing;
using BackEasing = easing::EasingGroup<easing::EaseBack>;
using BounceEasing = easing::EasingGroup<easing::EaseBounce>;
using CircularEasing = easing::EasingGroup<easing::EaseCircular>;
using CubicEasing = easing::EasingGroup<easing::EaseCubic>;
using ElasticEasing = easing::EasingGroup<easing::EaseElastic>;
using ExponentialEasing = easing::EasingGroup<easing::EaseExponential>;
using LinearEasing = easing::EasingGroup<easing::EaseLinear>;
using QuadraticEasing = easing::EasingGroup<easing::EaseQuadratic>;
using QuarticEasing = easing::EasingGroup<easing::EaseQuartic>;
using QuinticEasing = easing::EasingGroup<easing::EaseQuintic>;
using SinusoidalEasing = easing::EasingGroup<easing::EaseSinusoidal>;

}  // namespace takram

#endif  // TAKRAM_EASING_H_
