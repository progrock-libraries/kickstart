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

#include <kickstart/core/collection-util.hpp>       // int_size, begin_ptr_of
#include <kickstart/core/failure-handling.hpp>

#include <optional>
#include <string>
#include <string_view>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::failure_handling;        // hopefully etc.
    using namespace kickstart::collection_util;         // int_size, begin_ptr_of
    using   std::optional,
        std::string,
        std::string_view;

    class Console
    {
        virtual void write_bytes( const string_view& s ) = 0;
        virtual auto read_byte() -> int = 0;

    protected:
        Console() {}

    public:
        static inline auto instance() -> Console&;

        void output( const string_view& s )
        {
            assert( s.size() <= size_t( INT_MAX ) );
            if( s.size() == 0 ) {
                return;
            }
            write_bytes( s );
        }

        auto any_input()
            -> optional<string>
        {
            string  line;
            int     code;

            while( (code = read_byte()) != EOF and code != '\n' ) {
                line += char( code );
            }

            if( code == EOF and line.empty() ) {
                return {};
            }
            return line;
        }

        auto input() -> string
        {
            optional<string> result = any_input();
            hopefully( result.has_value() )
                or KS_FAIL_( End_of_file_exception, "At end of file." );
            return move( result.value() );
        }

        auto input( const string_view& prompt )
            -> string
        {
            output( prompt );
            return input();
        }
    };

    inline auto the_console()
        -> Console&
    { return Console::instance(); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Console,
        d::the_console;
    }  // namespace exported_names
}  // namespace kickstart::system_specific::_definitions

namespace kickstart::system_specific    { using namespace _definitions::exported_names; }
