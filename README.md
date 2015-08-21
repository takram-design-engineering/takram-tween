Tween
=====

A C++ library for time-based and frame-based tweening

[![Build Status](https://travis-ci.org/takram-design-engineering/takram-tween.svg)](https://travis-ci.org/takram-design-engineering/takram-tween)

## Tween Module

A tween is a representation of an interpolation for the specific time interval on a timeline. A timeline manages absolute time points and updates tweens. Tweens are always paired with a unique timeline.

Time interval in this library is either floating point value of seconds or the number of frames. A timeline is a specialization of either one, and cannot mix both in a single timeline. A frame-based tween can only managed by frame-based timelines, not by time-based timelines.

To run tweens, you need a timeline and usually a timeline host. [`takram::tween::TimelineHost`](/src/takram/tween/timeline_host.h) gives you a minimal interface to handle timelines and tweens. A timeline has an internal clock which is updated by calling the `advance` function of the timeline. Advancing the internal clock will update all of the tweens which belong to the timeline, applying interpolated values to the targets. Although it’s not supplied in this library, timelines should be advanced periodically by timers, threads or someway provided on your environment.

### Classes

- [`takram::tween::Interval`](src/takram/tween/interval.h)
- [`takram::tween::Clock`](src/takram/tween/clock.h)
- [`takram::tween::Timeline`](src/takram/tween/timeline.h)
- [`takram::tween::TimelineHost`](src/takram/tween/timeline_host.h)
- [`takram::tween::Tween`](src/takram/tween/tween.h)

### Pointer Target

```
Tween(target, to, easing, duration [, timeline ])
Tween(target, to, easing, duration [, callback [, timeline ]])
Tween(target, to, easing, duration, delay [, timeline ])
Tween(target, to, easing, duration, delay [, callback [, timeline ]])
```

- *target*
    - Pointer to a target variable
- *to*
    - Ending value of the *target*
- *easing*
    - An easing function
    - This must be a type, not a function pointer so that type-deduction can perform.
- *duration*
    - Duration for tweening represented by either time or frame interval
- *delay*
    - Delay to start tweening represented by either time or frame interval
- *callback*
    - A function to be invoked on the end of the tween
    - This must be a type, not a function pointer so that type-deduction can perform.
- *timeline*
    - A timeline this tween should belong to

#### Example

```cpp
Host host;
double value = 0.0;

// Tweens the value to 1.0 for 0.1 seconds using quadratic easing.
host.tween(&value, 1.0, takram::tween::QuadraticEasing::In, 0.1);

// Tweens the value to 1.0 for 0.2 seconds after 0.1 seconds delay using
// elastic easing.
host.tween(&value, 1.0, takram::tween::ElasticEasing::InOut, 0.2, 0.1);

// Tweens the value to 1.0 for 0.1 seconds using quadratic easing, and then to
// 0.0 for 0.1 seconds using quintic easing.
host.tween(&value, 1.0, takram::tween::QuadraticEasing::Out, 0.1, [&host]() {
  host.tween(&value, 0.0, takram::tween::QuinticEasing::Out, 0.1);
});
```

### Accessor Target

```
Tween(target, getter, setter, name, to, easing, duration [, timeline ])
Tween(target, getter, setter, name, to, easing, duration [, callback [, timeline ]])
Tween(target, getter, setter, name, to, easing, duration, delay [, timeline ])
Tween(target, getter, setter, name, to, easing, duration, delay [, callback [, timeline ]])
```

- *target*
    - Pointer to a target variable
- *getter*
    - Pointer to a member function that returns *target*’s property value
- *setter*
    - Pointer to a member function that changes *target*’s property value
- *name*
    - Arbitrary name that identifies the *target*’s property
    - By default, adding a tween will overwrite any preceding tweens of the same target to avoid race of changing the *target*’s property. However, because there is no way to reliably identify member function’s pointers, this will be the only hint to identify the *target*’s property coupled with the given *getter* and *setter*. It is your responsibility to maintain consistency of *name* across multiple tweens for the same properties.
- *to*
    - The ending value of the *target*’s property
- *easing*
    - An easing function
    - This must be a type, not a function pointer so that type-deduction can perform.
- *duration*
    - Duration for tweening represented by either time or frame interval
- *delay*
    - Delay to start tweening represented by either time or frame interval
- *callback*
    - A function to be invoked on the end of the tween
    - This must be a type, not a function pointer so that type-deduction can perform.
- *timeline*
    - A timeline this tween should belong to

```cpp
class Type final {
 public:
  Type() = default;
  explicit Type(double value) : value_(value) {}
  Type(const Type& other) = default;
  double value() const { return value_; }
  void set_value(double value) { value_ = value; }

 private:
  double value_;
};

Host host;
Type value(0.0);

// Tweens the value to 1.0 for 0.1 seconds using quadratic easing.
host.tween(&value, &Type::value, &type::set_value, "value", 1.0,
           takram::tween::QuadraticEasing::In, 0.1);

// Tweens the value to 1.0 for 0.2 seconds after 0.1 seconds delay using
// elastic easing.
host.tween(&value, &Type::value, &type::set_value, "value", 1.0,
           takram::tween::ElasticEasing::InOut, 0.2, 0.1);

// Tweens the value to 1.0 for 0.1 seconds using quadratic easing, and then to
// 0.0 for 0.1 seconds using quintic easing.
host.tween(&value, &Type::value, &type::set_value, "value", 1.0,
           takram::tween::QuadraticEasing::Out, 0.1, [&host]() {
  host.tween(&value, &Type::value, &type::set_value, "value", 0.0,
             takram::tween::QuinticEasing::Out, 0.1);
});
```

## Easing Module

An easing is given as a specialization of `takram::easing::Group`, in which `In`, `Out` and `InOut` static functors wrap the easing function. An easing function will be invoked with an unit time `t`, which is normalized between 0.0 and 1.0, and returns the corresponding normalized value for `t` (usually continuous for most of the range, and the returned value should be `t` at 0.0 and 1.0).

To make your own easing, simply write a function that takes `t`:

```cpp
template <class T>
struct Custom {
  T operator()(T parameter) {
    return /* Write your own logic */;
  }
};

using Easing = takram::easing::Group<Custom<takram::tween::Unit>>;
```

This enables ease-in, ease-out and ease-in-out out of your easing function.

```cpp
// Given:
T value;
const T to;
const double duration;

// Then:
takram::Tween<>(&value, to, Easing::In, duration);
takram::Tween<>(&value, to, Easing::Out, duration);
takram::Tween<>(&value, to, Easing::InOut, duration);
```

The pre-defined easings are derived from [Robert Penner’s Easing Functions](http://www.robertpenner.com/easing/) as listed below:

- [`takram::easing::BackEasing`](src/takram/easing/preset/back.h)
- [`takram::easing::BounceEasing`](src/takram/easing/preset/bounce.h)
- [`takram::easing::CircularEasing`](src/takram/easing/preset/circular.h)
- [`takram::easing::CubicEasing`](src/takram/easing/preset/cubic.h)
- [`takram::easing::ElasticEasing`](src/takram/easing/preset/elastic.h)
- [`takram::easing::ExponentialEasing`](src/takram/easing/preset/exponential.h)
- [`takram::easing::LinearEasing`](src/takram/easing/preset/linear.h)
- [`takram::easing::QuadraticEasing`](src/takram/easing/preset/quadratic.h)
- [`takram::easing::QuarticEasing`](src/takram/easing/preset/quartic.h)
- [`takram::easing::QuinticEasing`](src/takram/easing/preset/quintic.h)
- [`takram::easing::SinusoidalEasing`](src/takram/easing/preset/sinusoidal.h)

## Setup Guide

Run "setup.sh" inside "script" directory to initialize submodules and build dependant libraries.

### Submodules

- [Google Testing Framework](https://chromium.googlesource.com/external/googletest)

## License

MIT License

Copyright (C) 2014-2015 takram design engineering

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
