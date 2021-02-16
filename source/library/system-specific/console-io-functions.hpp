// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
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

#include <stdio.h>

#include <string>
#include <string_view>

namespace kickstart::system_specific::_definitions {
    using   std::string,
            std::string_view;

    using C_file = FILE*;

    inline auto is_console( const C_file f ) -> bool;
    inline void raw_output_to_console( const C_file, const string_view& );
    inline auto raw_input_from_console( const C_file ) -> string;
}  // namespace kickstart::system_specific::_definitions

namespace kickstart::system_specific {
    using   std::string,
        std::string_view;

    using C_file = FILE*;

    using _definitions::is_console;

    inline void output_to_console( const C_file f, const string_view& s )
    {
        #ifdef KS_CHECK_CONSOLE_PLEASE
            assert( is_console( f ) );
        #endif
        _definitions::raw_output_to_console( f, s );
    }

    inline auto input_from_console( const C_file f )
        -> string
    {
        #ifdef KS_CHECK_CONSOLE_PLEASE
            assert( is_console( f ) );
        #endif
        return _definitions::raw_input_from_console( f );
    }
}  // namespace kickstart::system_specific

#include <kickstart/system-specific/os-detection.hpp>
#if defined( xKS_OS_IS_WIN64 )
#   include <kickstart/system-specific/windows/console-io-functions.impl.hpp>
#else
#   include <kickstart/core/failure-handling.hpp>      // KS_FAIL
    namespace kickstart::system_specific::_definitions {
        inline auto is_console( const C_file )
            -> bool
        { return false; }

        inline void raw_output_to_console( const C_file, const string_view& ) {}
        inline auto raw_input_from_console( const C_file ) -> string { return ""; }
    }  // namespace kickstart::system_specific::_definitions
#endif
