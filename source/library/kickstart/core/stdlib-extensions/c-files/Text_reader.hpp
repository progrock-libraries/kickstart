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

#include <kickstart/core/stdlib-extensions/c-files/Abstract_c_file.hpp>
#include <kickstart/core/stdlib-extensions/filesystem/Path.hpp>
#include <kickstart/core/text-conversion/to-text/string-output-operator.hpp>

namespace kickstart::c_files::_definitions {
    using namespace kickstart::text_conversion;     // ""s, string operator<<

    class Text_reader:
        public Abstract_c_file
    {
    public:
        explicit Text_reader( const fsx::Path& path ):
            Abstract_c_file( open_c_file_or_x( path, "r" ) )
        {}

        auto input_or_none()
            -> optional<string>
        { return clib_input_or_none_from( c_file() ); }

        auto input()
            -> string
        {
            if( optional<string> s = input_or_none() ) {
                return move( s.value() );
            }
            KS_FAIL_( End_of_file_exception, "End of file" );       // TODO: check for other fail.
            unreachable();
        }

        auto has_passed_eof() const
            -> Truth
        { return !!::feof( c_file() ); }
    };

    namespace d = _definitions;
    namespace exports{ using
        d::Text_reader;
    }  // exports
}  // namespace kickstart::c_files::_definitions

namespace kickstart::c_files    { using namespace _definitions::exports; }
