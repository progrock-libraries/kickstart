// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
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

#include <kickstart/main_library/console/utf8-io.hpp>
#include <kickstart/main_library/core/ns▸collection_support.hpp>     // ssize_
#include <kickstart/main_library/core/failure-handling.hpp>                 // hopefully, fail, …
#include <kickstart/main_library/core/ns▸generate.hpp>               // generate::Relational_operators_from_compare_, …
#include <kickstart/main_library/core/ns▸language.hpp>               // Size etc.
#include <kickstart/main_library/core//ns▸matrices.hpp>                         // Matrix_ etc.
#include <kickstart/main_library/core/ns▸process.hpp>                          // process::Commandline
#include <kickstart/main_library/core/stdlib-extensions.hpp>                // bits_per, …
#include <kickstart/main_library/core/ns▸text_conversion.hpp>
#include <kickstart/main_library/core/text-encoding.hpp>

//TODO: find out about this
#include <kickstart/main_library/core/ns▸text_conversion/to-text/string-output-operator.hpp>

namespace kickstart::core {
    using namespace kickstart::             collection_support;         // <core/collection-util.hpp>
    using namespace kickstart::             failure_handling;           // <core/failure-handling.hpp>
    namespace generate = kickstart::        generate;
    using namespace kickstart::             language;                   // <core/ns▸ns▸language.hpp>
    using namespace kickstart::             matrices;                   // <core//ns▸matrices.hpp>
    namespace process = kickstart::         process;                    // <core/ns▸process.hpp>
    inline namespace                        stdlib_extensions {         // <core/stdlib-extensions.hpp>
        using namespace kickstart::             arrays;                 //   <… ns▸arrays.hpp>
        using namespace kickstart::             c_files;                //   <… c-files.hpp>
        namespace fsx = kickstart::             fsx;                    //   <… ns▸fsx.hpp>
        using namespace kickstart::             basic_type_properties;  //   <… ns▸basic_type_properties.hpp>
        namespace calc = kickstart::            calc;                   //   <… ns▸calc.hpp>
        namespace std_exception = kickstart::std_exception;             //   <… ns▸std_exceptions.hpp>
        using namespace kickstart::             strings;                //   <… /ns▸strings.hpp>
        using namespace kickstart::             type_traits;            //   <… ns▸type_traits.hpp>
        using namespace kickstart::             x_utility;              //   <… utility.hpp>
    }
    using namespace kickstart::             text_conversion;            // <core/ns▸text_conversion.hpp>
    using namespace kickstart::             string_output_operator;     // <core/ns▸text_conversion.hpp>
    namespace string_output_operator = kickstart::string_output_operator;
    inline namespace                        text_encoding {             // <core/text-encoding.hpp>
        namespace ascii = kickstart::           ascii;                  //   <… /ns▸ascii.hpp>
        namespace utf8 = kickstart::            utf8;                   //   <… ns▸utf8.hpp>
    }
    namespace utf8_io = kickstart::         utf8_io;                    // <console/utf8-io.hpp>
}  // namespace kickstart::core
