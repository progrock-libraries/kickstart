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

#include <kickstart/main_library/core/namespace▸language/types/Truth.hpp>
#include <kickstart/main_library/core/stdlib-extensions//namespace▸strings.hpp>
#include <kickstart/main_library/core/namespace▸text_conversion/to-text/string-output-operator.hpp>
#include <kickstart/main_library/core/text-encoding/namespace▸ascii/string-util.hpp>

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace kickstart::text_stream_io::_definitions {
    using namespace kickstart::text_conversion;     // ""s, string operator<<
    using   kickstart::language::Truth,
            kickstart::strings::split_on_whitespace;
    using   std::function,
            std::optional,
            std::string,
            std::string_view,
            std::vector;

    class Abstract_text_parts_reader
    {
    public:
        using Stop_checker      = auto( const string_view& s ) -> Truth;

    private:
        using Self              = Abstract_text_parts_reader;
        using String_views      = vector<string_view>;
        using View_iterator     = String_views::const_iterator;

        string                  m_line;
        String_views            m_line_parts;
        View_iterator           m_current;
        function<Stop_checker>  m_should_stop;

        virtual auto input_line() -> optional<string> = 0;

    protected:
        Abstract_text_parts_reader( Self&& ) = default;
        auto operator=( Self&& ) -> Self& = default;

    public:
        static auto non_stop( const string_view& )
            -> Truth
        { return false; }

        static auto stop_on_empty_line( const string_view& s )
            -> Truth
        { return (ascii::trimmed_view( s ).length() == 0); }

        virtual ~Abstract_text_parts_reader() = default;

        Abstract_text_parts_reader( function<Stop_checker> stop_checker = stop_on_empty_line ):
            m_line_parts(),
            m_current( m_line_parts.cend() ),
            m_should_stop( stop_checker )
        {}

        auto input_part()
            -> optional<string_view>
        {
            while( m_current == m_line_parts.cend() ) {
                optional<string> line = input_line();
                if( line.has_value() and not m_should_stop( line.value() ) ) {
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

        auto input_part_view_or_none() -> optional<string_view> { return input_part(); }
    };

    namespace d = _definitions;
    namespace exports{ using
        d::Abstract_text_parts_reader;
    }  // exports
}  // namespace kickstart::text_stream_io::_definitions

namespace kickstart::text_stream_io   { using namespace _definitions::exports; }
