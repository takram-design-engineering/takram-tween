#!/bin/sh
#
#  cpplint.sh
#
#  MIT License
#
#  Copyright (C) 2013-2014 Shota Matsuda
#  Copyright (C) 2014 takram design engineering
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
#

readonly SRCROOT="$(cd "$(dirname "$0")/../"; pwd)"
readonly CPPLINT="${SRCROOT}/cpplint/cpplint/cpplint.py"
readonly FILTERS="-readability/streams"
readonly PATTERN=".*/[a-z0-9_]*\."

for file in $(find -E "${SRCROOT}/src" -type f -regex "${PATTERN}(h|cc)$"); do
  python "${CPPLINT}" --root "src" --filter="${FILTERS}" "${file}"
done
if [[ -f "${SRCROOT}/include" ]]; then
  for file in $(find -E "${SRCROOT}/include" -type f -regex "${PATTERN}h$"); do
    python "${CPPLINT}" --root "include" --filter="${FILTERS}" "${file}"
  done
fi
if [[ -f "${SRCROOT}/test" ]]; then
  for file in $(find -E "${SRCROOT}/test" -type f -regex "${PATTERN}cc$"); do
    python "${CPPLINT}" --root "test" --filter="${FILTERS}" "${file}"
  done
fi
