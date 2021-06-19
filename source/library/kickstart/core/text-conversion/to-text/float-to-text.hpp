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

#include <kickstart/c/stdio.hpp>    // snprintf

#include <string>

namespace kickstart::text_conversion::_definitions {
    using   std::string;

    const int default_n_decimals = 6;       // In the C and C++ standard libraries.

    inline auto to_fixed(
        const double    value,
        const int       n_decimals = default_n_decimals
        ) -> string
    {
        const int buf_size = 1 + ::snprintf( nullptr, 0, "%.*f", n_decimals, value );
        auto result = string( buf_size, '\0' );
        const int n_chars = ::snprintf( result.data(), result.size(), "%.*f", n_decimals, value );
        result.resize( n_chars );
        return result;
    }

    inline auto to_scientific(
        const double    value,
        const int       n_decimals = default_n_decimals
        ) -> string
    {
        //                   -   1   .   1234567890   e   -  111     \0
        const int buf_size = 1 + 1 + 1 + n_decimals + 1 + 1 +  3    + 1;

        auto result = string( buf_size, '\0' );
        const int n_chars = ::snprintf( result.data(), result.size(), "%.*e", n_decimals, value );
        result.resize( n_chars );
        return result;
    }

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::default_n_decimals,
        d::to_fixed,
        d::to_scientific;
    }  // namespace exported names
}  // namespace kickstart::text_conversion::_definitions

namespace kickstart::text_conversion    { using namespace _definitions::exported_names; }
