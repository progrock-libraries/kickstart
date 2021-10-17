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

#include <kickstart/main_library/console/utf8-io/io.hpp>
#include <kickstart/main_library/core/ns▸language/syntax-support/Default.hpp>
#include <kickstart/main_library/core/stdlib-extensions/files/ns▸c_files/C_file_operations.hpp>
#include <kickstart/main_library/core/stdlib-extensions/files/ns▸text_stream_io/Abstract_text_parts_reader.hpp>

#include <functional>
#include <optional>
#include <string>
#include <utility>

namespace kickstart::console::_definitions {
    namespace k = kickstart;
    using   k::c_files::C_file, k::c_files::C_file_operations,
            k::text_stream_io::Abstract_text_parts_reader,
            k::utf8_io::output;
    using   std::function,
            std::optional,
            std::string,
            std::move;

    class Text_parts_reader final:
        private C_file_operations,
        public Abstract_text_parts_reader
    {
        using Self = Text_parts_reader;

        string                  m_line_prompt;

        auto input_line()
            -> optional<string>
            override
        {
            if( not m_line_prompt.empty() ) {
                output( m_line_prompt );
            }
            return C_file_operations::input_or_none();
        }

    public:
        Text_parts_reader(
            string                  line_prompt     = "",
            function<Stop_checker>  should_stop     = stop_on_empty_line
            ):
            C_file_operations( stdin ),
            Abstract_text_parts_reader( move( should_stop ) ),
            m_line_prompt( move( line_prompt ) )
        {}

        Text_parts_reader( Self&& ) = default;
        auto operator=( Self&& ) -> Self& = default;

        using C_file_operations::has_passed_eof;
    };

    namespace d = _definitions;
    namespace exports{ using
        d::Text_parts_reader;
    }  // exports
}  // namespace kickstart::console::_definitions

namespace kickstart::console    { using namespace _definitions::exports; }
