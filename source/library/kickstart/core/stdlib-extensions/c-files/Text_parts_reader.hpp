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
#include <kickstart/core/stdlib-extensions/strings.hpp>
#include <kickstart/core/text-conversion/to-text/string-output-operator.hpp>

#include <vector>

namespace kickstart::c_files::_definitions {
    using namespace kickstart::text_conversion;     // ""s, string operator<<
    using   kickstart::strings::split_on_whitespace;
    using   std::vector;

    class Text_parts_reader:
        private Abstract_c_file
    {
        using Self              = Text_parts_reader;
        using String_views      = vector<string_view>;
        using View_iterator     = String_views::const_iterator;

        string              m_line;
        String_views        m_line_parts;
        View_iterator       m_current;

    public:
        ~Text_parts_reader()
        {
            Abstract_c_file::release();
        }

        Text_parts_reader( const C_file f = stdin):
            Abstract_c_file( f ),
            m_line_parts(),
            m_current( m_line_parts.cend() )
        {}

        Text_parts_reader( Self&& other ):
            Abstract_c_file( move( other ) )
        {}

        auto operator=( Self&& other ) noexcept
            -> Self&
        {
            Abstract_c_file::operator=( move( other ) );
            return *this;
        }

        auto input_part_view_or_none()
            -> optional<string_view>
        {
            while( m_current == m_line_parts.cend() ) {
                if( optional<string> line = clib_input_or_none_from( c_file() ) ) {
                    m_line = line.value();
                    m_line_parts = split_on_whitespace( m_line );
                    m_current = m_line_parts.cbegin();
                } else {
                    return {};
                }
            }
            return *m_current++;
        }

        auto input_part_view() -> string_view   { return input_part_view_or_none().value(); }
        auto input_part_string() -> string      { return string( input_part_view() ); }

        auto input_part() -> string_view        { return input_part_view(); }

        using Abstract_c_file::has_passed_eof;
    };

    namespace d = _definitions;
    namespace exports{ using
        d::Text_parts_reader;
    }  // exports
}  // namespace kickstart::c_files::_definitions

namespace kickstart::c_files    { using namespace _definitions::exports; }
