//
//  takram/easing/group.h
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
#ifndef TAKRAM_EASING_GROUP_H_
#define TAKRAM_EASING_GROUP_H_

#include <functional>

#include "takram/easing/inflector.h"
#include "takram/easing/reverse_inflector.h"
#include "takram/easing/symmetric_inflector.h"

namespace takram {
namespace easing {

template <class T, template <class = T> class Function>
struct Group {
  static Inflector<T, Function> In;
  static ReverseInflector<T, Function> Out;
  static SymmetricInflector<T, Function> InOut;
};

#pragma mark -

template <class T, template <class> class Function>
Inflector<T, Function> Group<T, Function>::In;
template <class T, template <class> class Function>
ReverseInflector<T, Function> Group<T, Function>::Out;
template <class T, template <class> class Function>
SymmetricInflector<T, Function> Group<T, Function>::InOut;

}  // namespace easing
}  // namespace takram

#endif  // TAKRAM_EASING_GROUP_H_
