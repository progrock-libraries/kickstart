// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../assertion-headers/$-assert-reasonable-compiler.hpp"

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

#include "module-interface.hpp"

#include "../../core/failure-handling.hpp"              // hopefully, fail
#include "../../core/language/collection-util.hpp"      // end_ptr_of
#include "../../core/language/type_aliases.hpp"         // Type_
#include "../../core/text_conversion/to-text/string-output-operator.hpp"
#include "../../core/text-encoding-ascii/character-util.hpp"

#include <assert.h>
#include <fstream>

namespace kickstart::process::_definitions {
    using   std::string,
            std::vector,
            std::ifstream;

    using namespace kickstart::text_conversion;     // "<<" string builder.

    class Linux_commandline
        : public Commandline
    {
        friend auto Commandline::singleton() -> const Commandline&;

        Linux_commandline()
        {
            const auto& path = "/proc/self/cmdline";    // “self” provides getpid()
            ifstream f( path );
            hopefully( not f.fail() )
                or KS_FAIL( ""s << "failed to open “" << path << "”" );
            string command_line;
            getline( f, command_line )
                or fail( ""s << "failed to read “" << path << "”" );

            for( const char ch: command_line ) {
                if( ch == '\\' or is( ascii::space, ch ) ) {
                    m_command_line += '\\';
                }
                m_command_line += (ch == '\0'? ' ' : ch);
            }
            m_command_line.pop_back();      // A final ASCII zero translated to space.

            for( char const* p = command_line.data(); *p; p += strlen( p ) + 1 ) {
                    m_parts.push_back( string( p ) );
            }

            for( const auto& s: m_parts ) {
                m_part_views.push_back( string_view( s ) );
            }
        }
    };

    inline auto Commandline::singleton()
        -> const Commandline&
    {
        static Linux_commandline the_instance;
        return the_instance;
    }
}  // namespace kickstart::process::_definitions
