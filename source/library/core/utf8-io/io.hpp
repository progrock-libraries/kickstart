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


#include <kickstart/core/failure-handling.hpp>          // hopefully, fail
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/core/language/type-aliases.hpp>     // Size etc.
#include <kickstart/core/utf8-io/wrapped-c-tty-streams.hpp>

#include <stdio.h>          // stdin, stdout, stdcerr, ...

#include <stdexcept>
#include <optional>
#include <string>
#include <string_view>

namespace kickstart::utf8_io::_definitions {
    using namespace kickstart::failure_handling;    // hopefully, fail
    using namespace kickstart::language;            // Truth, Size etc.
    using namespace kickstart::utf8_io;

    using   std::runtime_error,
            std::optional,
            std::string,
            std::string_view;

    inline auto input_or_eof()
        -> optional<string>
    { return the_c_tty_streams().in.input_or_eof(); }

    inline auto input()
        -> string
    { return the_c_tty_streams().in.input(); }

    inline void output( const string_view& s )
    {
        the_c_tty_streams().out.output( s );
    }

    inline void output_error_message( const string_view& s )
    {
        auto& err_stream = the_c_tty_streams().err;
        err_stream.output( s );
        err_stream.flush();
    }

    inline auto input( const string_view& prompt )
        -> string
    {
        output( prompt );
        return input();
    }

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::End_of_file_exception,       // Re-exported here for convenience.
        d::input_or_eof,
        d::input,
        d::output,
        d::output_error_message;
    }  // namespace exported names
}  // namespace kickstart::utf8_io::_definitions

namespace kickstart::utf8_io    { using namespace _definitions::exported_names; }
