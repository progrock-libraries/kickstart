// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../assertion-headers/assert-reasonable-compiler.hpp"

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


#include "standard_streams.hpp"                     // standard_streams::*
#include "../../core/failure-handling.hpp"          // hopefully, fail
#include "../../core/language/type_aliases.hpp"     // Size etc.

#include <stdio.h>          // stdin, stdout, stdcerr, ...

#include <optional>
#include <string>
#include <string_view>

namespace kickstart::utf8_io::_definitions {
    using namespace kickstart::failure_handling;    // hopefully, fail
    using namespace kickstart::language;            // Size etc.

    using   std::optional,
            std::string,
            std::string_view;

    using C_file_ptr = FILE*;

    inline void flush( const C_file_ptr f = stdout )
    {
        assert( f != stdin );
        hopefully( f != stdin )
            or KS_FAIL_( logic_error,
                "Flushing `stdin` (or any input stream) is Undefined Behavior."
            );
        ::fflush( f );
    }

    inline auto output_to( const C_file_ptr f, const string_view& s )
        -> bool
    {
        const Size n = s.length();
        if( n <= 0 ) {
            return true;
        }

        auto& utf8_fwrite = standard_streams::singleton().write_func_for( f );
        const Size n_written = utf8_fwrite( &*s.begin(), n, f );
        return n_written == n;
    }

    inline auto output( const string_view& s )
        -> bool
    { return output_to( stdout, s ); }

    inline auto output_error_message( const string_view& s )
        -> bool
    {
        const bool success = output_to( stderr, s );
        if( success ) {
            flush( stderr );
        }
        return success;
    }

    inline auto any_input_from( const C_file_ptr f )
        -> optional<string>
    {
        auto& utf8_fgetc = standard_streams::singleton().read_byte_func_for( f );

        string  line;
        int     code;
        while( (code = utf8_fgetc( f )) != EOF and code != '\n' ) {
            line += char( code );
        }
        hopefully( not ::ferror( f ) )  or KS_FAIL( "::fgetc failed" );
        if( code == EOF and line.empty() ) {
            return {};
        }
        return line;
    }

    inline auto input_from( const C_file_ptr f )
        -> string
    {
        optional<string> input = any_input_from( f );
        hopefully( input.has_value() ) or KS_FAIL( "At end of file." );
        return move( input.value() );
    }

    inline auto any_input()
        -> optional<string>
    { return any_input_from( stdin ); }

    inline auto input()
        -> string
    { return input_from( stdin ); }

    inline auto input( const string_view& prompt )
        -> string
    {
        output( prompt );
        return input();
    }

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::flush,
        d::output_to,
        d::output,
        d::output_error_message,
        d::any_input_from,
        d::input_from,
        d::any_input,
        d::input;
    }  // namespace exported names
}  // namespace kickstart::utf8_io::_definitions

namespace kickstart::utf8_io    { using namespace _definitions::exported_names; }
