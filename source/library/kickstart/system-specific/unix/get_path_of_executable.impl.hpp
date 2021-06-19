// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/assertion-headers/~assert-reasonable-compiler.hpp>
#ifndef KS_OS_IS_UNIX
#   error "This header is for Unix systems only."
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

#include <kickstart/core/failure-handling.hpp>
#include <kickstart/core/language/type-aliases.hpp>
#include <kickstart/core/text-conversion/to-text/string-output-operator.hpp>

#include <unistd.h>         // getpid

#include <assert.h>
#include <stdio.h>          // fgets, popen, pclose

#include <iterator>         // std::size
#include <string>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::failure_handling;    // hopefully
    using namespace kickstart::language;            // C_str
    using namespace kickstart::text_conversion;     // ""s, operator<< for strings.

    using   std::size,
            std::string;

    // TODO: move to header with reusable support stuff
    // Source: <url: https://stackoverflow.com/a/478960>
    auto output_of( const C_str cmd)
        -> string
    {
        struct Pipe
        {
            FILE*   m_stream;
            char    m_buffer[128];

            ~Pipe() { ::pclose( m_stream ); }

            Pipe( const C_str cmd ):
                m_stream( ::popen( cmd, "r" ) )
            {
                hopefully( !!m_stream )
                    or KS_FAIL( "Failed to create pipe with `popen()`." );
            }

            auto next_string()
                -> C_str
            { return ::fgets( m_buffer, size( m_buffer ), m_stream ); }
        };

        std::string result;
        Pipe pipe( cmd );
        while( const C_str s = pipe.next_string() ) {
            result += s;
        }
        return result;
    }

    inline auto get_path_of_executable()
        -> string
    {
        const pid_t self_pid = ::getpid();
        const string command = ""s
            << "lsof -p" << self_pid << " | "
            << "awk '{if($4 == \"txt\") printf($9)}' "
            << "2>/dev/null";
        string result = output_of( command.c_str() );
        hopefully( result.length() > 0 )
            or KS_FAIL( "Zero length path from “" << command << "”." );
        return result;
    }
}  // namespace kickstart::system_specific::_definitions
