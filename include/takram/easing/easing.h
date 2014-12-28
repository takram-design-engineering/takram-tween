//
//  takram/easing/easing.h
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
#ifndef TAKRAM_EASING_EASING_H_
#define TAKRAM_EASING_EASING_H_

#include <functional>

namespace takram {
namespace easing {

using Easing = std::function<double(double)>;

template <double (*Function)(double)>
struct EasingGroup {
 private:
  struct In_ {
    double operator()(double t) const;
  };
  struct Out_ {
    double operator()(double t) const;
  };
  struct InOut_ {
    double operator()(double t) const;
  };

 public:
  static constexpr In_ In = In_();
  static constexpr Out_ Out = Out_();
  static constexpr InOut_ InOut = InOut_();
};

#pragma mark - Inline Implementations

template <double (*Function)(double)>
inline double EasingGroup<Function>::In_::operator()(double t) const {
  return Function(t);
}

template <double (*Function)(double)>
inline double EasingGroup<Function>::Out_::operator()(double t) const {
  return 1.0 - Function(1.0 - t);
}

template <double (*Function)(double)>
inline double EasingGroup<Function>::InOut_::operator()(double t) const {
  if (t < 0.5) {
    return In_()(2.0 * t) / 2.0;
  } else {
    return 0.5 + Out_()(2.0 * t - 1.0) / 2.0;
  }
}

template <double (*Function)(double)>
constexpr typename EasingGroup<Function>::In_ EasingGroup<Function>::In;
template <double (*Function)(double)>
constexpr typename EasingGroup<Function>::Out_ EasingGroup<Function>::Out;
template <double (*Function)(double)>
constexpr typename EasingGroup<Function>::InOut_ EasingGroup<Function>::InOut;

}  // namespace easing
}  // namespace takram

#endif  // TAKRAM_EASING_EASING_H_
