// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/assertion-headers/~assert-reasonable-compiler.hpp>
#ifndef KS_OS_IS_LINUX
#   error "This header is for Linux systems only."
#   include <nosuch>
#endif

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

#include <kickstart/main_library/system-specific/Commandline_data.hpp>

#include <kickstart/main_library/core/failure-handling.hpp>
#include <kickstart/main_library/core/namespace▸text_conversion/to-text/string-output-operator.hpp>
#include <kickstart/main_library/core/text-encoding/namespace▸ascii/character-util.hpp>

#include <kickstart/c/string.hpp>       // strlen

//TODO: Use C files.
#include <fstream>
#include <string>
#include <string_view>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::text_conversion;     // "<<" string builder.
    using namespace kickstart::failure_handling;    // hopefully, fail
    using namespace std::string_view_literals;

    using   std::ifstream,
            std::string,
            std::string_view;

    inline auto get_commandline_data()
        -> Commandline_data
    {
        const auto& path = "/proc/self/cmdline";    // “self” provides getpid()
        ifstream f( path );
        hopefully( not f.fail() )
            or KS_FAIL( ""s << "failed to open “" << path << "”" );
        string command_line;
        getline( f, command_line )
            or KS_FAIL( ""s << "failed to read “" << path << "”" );

        Commandline_data result;
        const auto npos = string_view::npos;
        for( const char ch: command_line ) {
            if( "\\<>()&|,;\"\'"sv.find( ch ) != npos or is( ascii::whitespace, ch ) ) {
                result.fulltext += '\\';
            }
            result.fulltext += (ch == '\0'? ' ' : ch);
        }
        result.fulltext.pop_back();     // A final ASCII zero translated to space.

        for( char const* p = command_line.data(); *p; p += strlen( p ) + 1 ) {
            result.parts.push_back( string( p ) );
        }
        return result;
    }
}  // namespace kickstart::system_specific::_definitions
