// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#ifndef KS_OS_IS_UNIX
#   error "This header is for Unix systems only."
#   include <nosuch>
#endif
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

#include <kickstart/root/system-specific/Console.hpp>

#include <kickstart/root/core/namespace.language/types/Truth.hpp>

#include <kickstart/c/assert.hpp>       // assert
#include <kickstart/c/limits.hpp>       // INT_MAX
#include <kickstart/c/stdio.hpp>        // FILE*

//#include <queue>
//#include <string>
//#include <utility>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::language;                // Truth etc.

    using C_file_ptr = FILE*;
    const auto& tty_path = "/dev/tty";

    inline auto open_console_input()
        -> C_file_ptr
    {
        const C_file_ptr f = fopen( tty_path, "r" );
        hopefully( f != 0 )
            or KS_FAIL( ""s << "Failed to open “" << tty_path << "” for input." );
        return f;
    }

    inline auto open_console_output()
        -> C_file_ptr
    {
        const C_file_ptr f = fopen( tty_path, "w" );
        hopefully( f != 0 )
            or KS_FAIL( ""s << "Failed to open “" << tty_path << "” for output." );
        return f;
    }

    class Unix_console:
        public Console
    {
        friend auto Console::instance() -> Console&;

        C_file_ptr  m_input     = {};
        C_file_ptr  m_output    = {};

        void write_bytes( const string_view& s ) override
        {
            const int n = int_size( s );
            const int n_written = fwrite( begin_ptr_of( s ), 1, n, m_output );
            hopefully( n_written == n )
                or KS_FAIL( "Failed to write to console." );
        }

        auto read_byte() -> int override
        {
            return fgetc( m_input );
        }

        Unix_console():
            m_input( open_console_input() ),
            m_output( open_console_output() )
        {
            setvbuf( m_output, nullptr, _IONBF, 0 );        // No output buffering, please.
        }
    };

    inline auto Console::instance()
        -> Console&
    {
        static Unix_console the_instance;
        return the_instance;
    }
}  // namespace kickstart::system_specific::_definitions
