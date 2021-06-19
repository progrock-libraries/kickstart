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

#include <kickstart/system-specific/os-detection.hpp>
#if defined( KS_OS_IS_WIN64 )
#   include <kickstart/system-specific/windows/console-adapted-io-functions.impl.hpp>
#elif defined( KS_OS_IS_UNIX )
#   include <kickstart/core/stdlib-extensions/c-files/wrapped-clib-io.hpp>
#   include <kickstart/core/language/Truth.hpp>
#   include <unistd.h>      // isatty
#   include <stdio.h>       // fileno
#   include <optional>
#   include <string>
#   include <string_view>
    namespace kickstart::system_specific::_definitions {
        using namespace c_files;
        using namespace language;
        using std::optional, std::string, std::string_view;

        inline auto is_console( const C_file f )
            -> Truth
        { return !!::isatty( fileno( f ) ); }

        inline auto raw_input_or_none_from_console( const C_file f )
            -> optional<string>
        { return clib_input_or_none_from( f ); }

        inline void raw_output_to_console( const C_file f, const string_view& s )
        {
            clib_output_to( f, s );
        }
    }  // namespace kickstart::system_specific::_definitions
#else
#   include <kickstart/core/stdlib-extensions/c-files/wrapped-clib-io.hpp>
#   include <kickstart/core/language/Truth.hpp>
#   include <optional>
#   include <string>
#   include <string_view>
    namespace kickstart::system_specific::_definitions {
        using namespace c_files;
        using namespace language;
        using std::optional, std::string, std::string_view;

        inline auto is_console( const C_file )
            -> Truth
        { return false; }

        inline auto raw_input_or_none_from_console( const C_file f )
            -> optional<string>
        { return clib_input_or_none_from( f ); }

        inline void raw_output_to_console( const C_file f, const string_view& s )
        {
            clib_output_to( f, s );
        }
    }  // namespace kickstart::system_specific::_definitions
#endif

#include <kickstart/system-specific/console-adapted-io-functions.interface.hpp>
