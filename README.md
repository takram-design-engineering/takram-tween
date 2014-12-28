Tween
=====

A C++ library for time-based and frame-based tweening

## Basic Usage

### Tween Module

### Easing Module

An easing is given as a specialization of `takram::easing::EasingGroup`, in which `In`, `Out` and `InOut` static functors wrap the easing function. An easing function will be invoked with an unit time `t`, which is normalized between 0.0 and 1.0, andreturn the corresponding normalized value for `t`. To make your own easing, write a function that takes `t`, then define a specialization:

```cc
inline double Ease(double t) {
  return /* Write your own logic */;
}
using Easing = takram::easing::EasingGroup<Ease>;

```

This enables you to make use of ease-in, ease-out and ease-in-out out of your easing function.

```cc
// Given:
T value;
const T to;
const double duration;

// Then:
takram::Tween<>(&value, to, Easing::In, duration);
takram::Tween<>(&value, to, Easing::Out, duration);
takram::Tween<>(&value, to, Easing::InOut, duration);
```

The pre-defined easings are derived from [Robert Penner’s Easing Functions](http://www.robertpenner.com/easing/) listed below:

- [`takram::BackEasing`](include/takram/easing.h)
- [`takram::BounceEasing`](include/takram/easing.h)
- [`takram::CircularEasing`](include/takram/easing.h)
- [`takram::CubicEasing`](include/takram/easing.h)
- [`takram::ElasticEasing`](include/takram/easing.h)
- [`takram::ExponentialEasing`](include/takram/easing.h)
- [`takram::LinearEasing`](include/takram/easing.h)
- [`takram::QuadraticEasing`](include/takram/easing.h)
- [`takram::QuarticEasing`](include/takram/easing.h)
- [`takram::QuinticEasing`](include/takram/easing.h)
- [`takram::SinusoidalEasing`](include/takram/easing.h)

## Setup Guide

Run "setup.sh" inside "scripts" directory to initialize submodules and build dependant libraries.

### Submodules

- [cpplint](https://github.com/sgss/mirror-cpplint)
- [gflags](https://code.google.com/p/gflags/)
- [glog](https://github.com/sgss/mirror-glog)
- [gtest](https://github.com/sgss/mirror-googletest)

## Style Guide

This project tries to conform to [Google's C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml) except:

- Use of lambda expressions
- Use of exceptions in exceptional cases
- Use of streams when strongly needed for convenience
- Naming convention of non-inline functions

## License

MIT License

Copyright (C) 2014 takram design engineering

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
