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

#include <kickstart/root/console/utf8-io/wrapped-c-tty-streams.hpp>

namespace kickstart::utf8_io::_definitions {
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

    inline auto tty_stream_input_or_none_from( const C_file f )
        -> optional<string>
    { return Temporary_c_tty_input_stream_wrapping( f ).input_or_none(); }

    inline auto tty_stream_input_from( const C_file f )
        -> string
    { return Temporary_c_tty_input_stream_wrapping( f ).input(); }

    inline void tty_stream_output_to( const C_file f, const string_view& s )
    {
        Temporary_c_tty_output_stream_wrapping( f ).output( s );
    }

    namespace d = _definitions;
    namespace exports{ using
        d::Temporary_c_tty_output_stream_wrapping,
        d::Temporary_c_tty_input_stream_wrapping,
        d::tty_stream_input_or_none_from,
        d::tty_stream_input_from,
        d::tty_stream_output_to;
    }  // exports
}  // namespace kickstart::utf8_io::_definitions

namespace kickstart::utf8_io    { using namespace _definitions::exports; }
