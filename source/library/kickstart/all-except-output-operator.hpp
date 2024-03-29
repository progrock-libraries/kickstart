﻿// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/assertion-headers/~assert-reasonable-compiler.hpp>

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

#include <kickstart/cpp-except-iostreams.hpp>       // Safe-with-`using` stuff from std library.
#include <kickstart/main_library/console.hpp>
#include <kickstart/main_library/core.hpp>

namespace kickstart::all_except_output_operator {
    using namespace core;               // From "core.hpp"
    using namespace console_startup;    // From "console.hpp"
    using namespace stdlib;             // From "cpp.hpp"
    using namespace utf8_io;            // From "console.hpp"

    namespace ansi_escape_seq       = kickstart::ansi_escape_sequences;
    namespace portable_dingbats     = kickstart::portable_dingbats;
    namespace process               = kickstart::process;
    namespace stdlib                = kickstart::stdlib;
}  // namespace kickstart::all_except_output_operator

namespace kickstart::all {
    using namespace all_except_output_operator;
}  // namespace kickstart::all

namespace progrock{ namespace basics = ::kickstart; }
