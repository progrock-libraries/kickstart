﻿// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/core/language/assertion-headers/~assert-reasonable-compiler.hpp>

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

#include <kickstart/core/collection-util.hpp>       // ssize_
#include <kickstart/core/failure-handling.hpp>      // hopefully, fail, …
#include <kickstart/core/generate.hpp>              // generate::Relops_from_compare_, …
#include <kickstart/core/language.hpp>              // Size etc.
#include <kickstart/core/large-integers.hpp>        // Uint_128
#include <kickstart/core/matrices.hpp>              // Matrix_ etc.
#include <kickstart/core/process.hpp>               // process::Commandline
#include <kickstart/core/stdlib-extensions.hpp>     // bits_per, …
#include <kickstart/core/text-conversion.hpp>
#include <kickstart/core/text-encoding.hpp>
#include <kickstart/core/utf8-io.hpp>

namespace kickstart::core {
    using namespace kickstart::             collection_util;            // <core/collection-util.hpp>
    using namespace kickstart::             failure_handling;           // <core/failure-handling.hpp>
    namespace generate = kickstart::        generate;
    using namespace kickstart::             language;                   // <core/language.hpp>
    using namespace kickstart::             large_integers;             // <core/large-integers.hpp>
    inline namespace                        language_support {
        using namespace kickstart::             stdlib;                 //   <stdlib-includes/basics.hpp>
    }
    using namespace kickstart::             matrices;                   // <core/matrices.hpp>
    namespace process = kickstart::         process;                    // <core/process.hpp>
    inline namespace                        stdlib_extensions {         // <core/stdlib-extensions.hpp>
        using namespace kickstart::             c_files;                //   <… c-files.hpp>
        namespace fsx = kickstart::             fsx;                    //   <… filesystem.hpp>
        using namespace kickstart::             limits;                 //   <… limits.hpp>
        namespace math = kickstart::            math;                   //   <… math.hpp>
        namespace std_exception = kickstart::std_exception;             //   <… standard-exceptions.hpp>
        using namespace kickstart::             strings;                //   <… strings.hpp>
        using namespace kickstart::             type_traits;            //   <… type-traits.hpp>
    }
    using namespace kickstart::             text_conversion;            // <core/text-conversion.hpp>
    inline namespace                        text_encoding {             // <core/text-encoding.hpp>
        namespace ascii = kickstart::           ascii;                  //   <… ascii.hpp>
        namespace utf8 = kickstart::            utf8;                   //   <… utf8.hpp>
    }
    namespace utf8_io = kickstart::         utf8_io;                    // <core/utf8-io.hpp>
}  // namespace kickstart::core
