// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../assertion-headers/~assert-reasonable-compiler.hpp"

// Copyright (c) 2020 Alf P. Steinbach. MIT license, with license text:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include    <stdexcept>

namespace kickstart::text_conversion {
    namespace exception {
        using Invalid_argument      = std::invalid_argument;
        using Out_of_range          = std::out_of_range;

        // The specification string is empty.
        class Empty_specification:
            public Invalid_argument
        { using Invalid_argument::Invalid_argument; };

        // There is unexpected valid spec (continuation) text after the spec.
        class Unexpected_spec_extension:
            public Invalid_argument
        { using Invalid_argument::Invalid_argument; };

        // An attempted number specification contains text (possibly just whitespace) at the end.
        class Unexpected_suffix_text:
            public Invalid_argument
        { using Invalid_argument::Invalid_argument; };

        // An integer specification contains decimals.
        class Decimals_specified_for_integer:
            public Invalid_argument
        { using Invalid_argument::Invalid_argument; };

        // The specified value is outside the number range of the result type.
        class Representable_range_exceeded:
            public Out_of_range
        { using Out_of_range::Out_of_range; };

        // The specified value is in the type's range but is unsuitable for the given purpose.
        class Unsuitable_value:
            public Out_of_range
        { using Out_of_range::Out_of_range; };
    }  // namespace exception
}  // namespace kickstart::text_conversion
