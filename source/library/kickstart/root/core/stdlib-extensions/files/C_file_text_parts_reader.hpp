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

#include <kickstart/root/core/stdlib-extensions/files/C_file_operations.hpp>
#include <kickstart/root/core/stream-io-util/Abstract_text_parts_reader.hpp>

namespace kickstart::c_files::_definitions {
    using kickstart::stream_io_util::Abstract_text_parts_reader;

    class C_file_text_parts_reader final:
        private C_file_operations,
        public Abstract_text_parts_reader
    {
        using Self = C_file_text_parts_reader;

        virtual auto input_line()
            -> optional<string> override
        { return C_file_operations::input_or_none(); }

    public:
        C_file_text_parts_reader( const C_file f ):
            C_file_operations( f )
        {}

        C_file_text_parts_reader( Self&& ) = default;
        auto operator=( Self&& ) -> Self& = default;

        // From Abstract_text_parts_reader:
        //
        // auto input_part_view_or_none() -> optional<string_view>;
        // auto input_part_view() -> string_view;
        // auto input_part_string() -> string;
        // auto input_part() -> string_view;

        using C_file_operations::has_passed_eof;
    };

    namespace d = _definitions;
    namespace exports{ using
        d::C_file_text_parts_reader;
    }  // exports
}  // namespace kickstart::c_files::_definitions

namespace kickstart::c_files    { using namespace _definitions::exports; }
