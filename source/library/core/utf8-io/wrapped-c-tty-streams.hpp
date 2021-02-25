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

#include <kickstart/core/stdlib-extensions/c-files/Wrapped_c_file.hpp>

#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace kickstart::utf8_io::_definitions {
    using namespace kickstart::language;            // Truth
    using namespace kickstart::c_files;
    using namespace kickstart::failure_handling;
    namespace ks = kickstart::system_specific;

    using   ks::is_console,
            ks::input_or_eof_from_console,
            ks::output_to_console;
    using   std::optional,
            std::string,
            std::string_view;

    using Input_func    = auto ( const C_file f ) -> optional<string>;
    using Output_func   = void ( const C_file f, const string_view& s );

    class C_tty_input_stream:
        public Wrapped_c_file
    {
        Input_func*     m_input_or_eof_from;

    public:
        C_tty_input_stream( const C_file f ):
            Wrapped_c_file( f ),
            m_input_or_eof_from(
                is_console( f )? input_or_eof_from_console : clib_input_or_eof_from
                )
        {}

        auto input_or_eof()
            -> optional<string>
        { return m_input_or_eof_from( c_file() ); }

        auto input()
            -> string
        {
            if( optional<string> result = input_or_eof() ) {
                return move( result.value() );
            }
            KS_FAIL_( End_of_file_exception, "Logical end of file encountered." );
            unreachable();
        }
    };

    class C_tty_output_stream:
        public Wrapped_c_file
    {
        Output_func*    m_output_to;

    public:
        C_tty_output_stream( const C_file f ):
            Wrapped_c_file( f ),
            m_output_to( is_console( f )? output_to_console : clib_output_to )
        {}

        void output( const string_view& s )
        {
            m_output_to( c_file(), s );
        }

        void flush()
        {
            ::fflush( c_file() );
        }
    };

    struct C_tty_streams
    {
        C_tty_input_stream&     std_in;
        C_tty_output_stream&    std_out;
        C_tty_output_stream&    std_err;
    };

    inline auto the_c_tty_streams()
        -> C_tty_streams&
    {
        static auto the_in_stream       = C_tty_input_stream( stdin );
        static auto the_out_stream      = C_tty_output_stream( stdout );
        static auto the_err_stream      = C_tty_output_stream( stderr );

        static auto the_streams         = C_tty_streams
        {
            the_in_stream, the_out_stream, the_err_stream
        };

        return the_streams;
    }

    class Temporary_c_tty_input_stream_wrapping:
        public C_tty_input_stream
    {
    public:
        ~Temporary_c_tty_input_stream_wrapping() { release(); }
        Temporary_c_tty_input_stream_wrapping( const C_file f ): C_tty_input_stream( f ) {}
        auto ref() -> C_tty_input_stream& { return *this; }
    };

    class Temporary_c_tty_output_stream_wrapping:
        public C_tty_output_stream
    {
    public:
        ~Temporary_c_tty_output_stream_wrapping() { release(); }
        Temporary_c_tty_output_stream_wrapping( const C_file f ): C_tty_output_stream( f ) {}
        auto ref() -> C_tty_output_stream& { return *this; }
    };

    inline auto tty_stream_input_or_eof_from( const C_file f )
        -> optional<string>
    { return Temporary_c_tty_input_stream_wrapping( f ).input_or_eof(); }

    inline auto tty_stream_input_from( const C_file f )
        -> string
    { return Temporary_c_tty_input_stream_wrapping( f ).input(); }

    inline void tty_stream_output_to( const C_file f, const string_view& s )
    {
        Temporary_c_tty_output_stream_wrapping( f ).output( s );
    }

    namespace d = _definitions;
    namespace exports{ using
        d::C_tty_input_stream,
        d::C_tty_output_stream,
        d::the_c_tty_streams,
        d::Temporary_c_tty_output_stream_wrapping,
        d::Temporary_c_tty_input_stream_wrapping,
        d::tty_stream_input_or_eof_from,
        d::tty_stream_input_from,
        d::tty_stream_output_to;
    }  // exports
}  // namespace kickstart::utf8_io::_definitions

namespace kickstart::utf8_io    { using namespace _definitions::exports; }
