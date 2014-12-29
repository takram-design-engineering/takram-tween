Tween
=====

A C++ library for time-based and frame-based tweening

## Basic Usage

### Tween Module

A tween is a representation of an interpolation for the specific time interval on a timeline. A timeline manages absolute time points and updates tweens. Tweens are always paired with a unique timeline.

Time interval in this library is either floating point value of seconds or the number of frames. A timeline is a specialization of either one, and cannot mix both in a single timeline. A frame-based tween can only managed by frame-based timelines, not by time-based timelines.

To run tweens, you need a timeline and usually a timeline host. [`takram::tween::TimelineHost`](/include/takram/tween/timeline_host.h) gives you a minimal interface to handle a timeline and tweens. A timeline has an internal clock which is updated by calling the `advance` function of timeline. Advancing the internal clock will update all of the tweens which belong to the timeline, applying interpolated values to the targets. Although it’s not supplied in this library, timelines should be advanced periodically by timer, threading or someway provided on your environment. 

The code below demonstrates a basic setup of the timeline host:

```cc
class Host : public takram::tween::TimelineHost<takram::tween::Time> {
 public:
  // Required by takram::tween::TimelineHost
  Timeline& timeline() override { return timeline_; }
  const Timeline& timeline() const override { return timeline_; }

  // Periodically-called function
  void update() {
    timeline_.advance();  // Advance the clock of the timeline
  }

 private:
  Timeline timeline_;  // Hosted timeline
};
```



#### Pointer Target

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
    
##### Example

```cc
Host host;
double value = 0.0;

// Tweens the value to 1.0 for 0.1 seconds using quadratic easing.
host.tween(&value, 1.0, takram::QuadraticEasing::In, 0.1);

// Tweens the value to 1.0 for 0.2 seconds after 0.1 seconds delay using
// elastic easing.
host.tween(&value, 1.0, takram::ElasticEasing::InOut, 0.2, 0.1);

// Tweens the value to 1.0 for 0.1 seconds using quadratic easing, and then to
// 0.0 for 0.1 seconds using quintic easing.
host.tween(&value, 1.0, takram::QuadraticEasing::Out, 0.1, [&host]() {
  host.tween(&value, 0.0, takram::QuinticEasing::Out, 0.1);
});
```

#### Accessor Target

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

```cc
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
           takram::QuadraticEasing::In, 0.1);

// Tweens the value to 1.0 for 0.2 seconds after 0.1 seconds delay using
// elastic easing.
host.tween(&value, &Type::value, &type::set_value, "value", 1.0, 
           takram::ElasticEasing::InOut, 0.2, 0.1);

// Tweens the value to 1.0 for 0.1 seconds using quadratic easing, and then to
// 0.0 for 0.1 seconds using quintic easing.
host.tween(&value, &Type::value, &type::set_value, "value", 1.0, 
           takram::QuadraticEasing::Out, 0.1, [&host]() {
  host.tween(&value, &Type::value, &type::set_value, "value", 0.0, 
             takram::QuinticEasing::Out, 0.1);
});
```

#### Classes

- [`takram::tween::Interval`](include/takram/tween/interval.h)
- [`takram::tween::Clock`](include/takram/tween/clock.h)
- [`takram::tween::Timeline`](include/takram/tween/timeline.h)
- [`takram::tween::TimelineHost`](include/takram/tween/timeline_host.h)
- [`takram::Tween`](include/takram/tween/tween.h)

### Easing Module

An easing is given as a specialization of `takram::easing::EasingGroup`, in which `In`, `Out` and `InOut` static functors wrap the easing function. An easing function will be invoked with an unit time `t`, which is normalized between 0.0 and 1.0, and returns the corresponding normalized value for `t` (usually continuous for most of the range, and the returned value should be `t` at 0.0 and 1.0).

To make your own easing, simply write a function that takes `t`:

```cc
inline double Ease(double t) {
  return /* Write your own logic */;
}
using Easing = takram::easing::EasingGroup<Ease>;
```

This enables ease-in, ease-out and ease-in-out out of your easing function.

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

The pre-defined easings are derived from [Robert Penner’s Easing Functions](http://www.robertpenner.com/easing/) as listed below:

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
