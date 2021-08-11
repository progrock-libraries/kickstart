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

#include <kickstart/root/core/namespace.collection_utility.hpp>        // int_size, begin_ptr_of
#include <kickstart/root/core/failure-handling.hpp>
#include <kickstart/root/core/namespace.language/types/Truth.hpp>
#include <kickstart/root/core/stdlib-extensions/strings.hpp> // for_each_part_of
#include <kickstart/root/core/text-encoding/namespace.utf8/bom.hpp>    // bom

#include <optional>
#include <string>
#include <string_view>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::collection_utility;     // int_size, begin_ptr_of
    using namespace kickstart::failure_handling;    // hopefully etc.
    using namespace kickstart::language;            // Truth
    using namespace kickstart::strings;             // for_each_part_of
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
            if( s.size() == 0 ) { return; }
            for_each_part_of( s, utf8::bom, [this]( const string_view& part ) {
                write_bytes( part );
            } );
        }

        auto input_or_none()
            -> optional<string>
        {
            string  line;
            int     code;

            while( (code = read_byte()) != EOF and code != '\n' ) {
                line += char( code );
            }
            const Truth eof = (line.empty() and code == EOF);
            if( eof ) { return {}; }
            return line;
        }

        auto input() -> string
        {
            optional<string> result = input_or_none();
            hopefully( result.has_value() )
                or KS_FAIL_( End_of_file_exception, "Logical end of file encountered." );
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
