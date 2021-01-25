// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../assertion-headers/$-assert-reasonable-compiler.hpp"

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

#include "../../core/language/collection-util.hpp"     // Array_span_

#include <string>
#include <string_view>
#include <vector>

namespace kickstart::process::_definitions {
    using   kickstart::language::Array_span_;
    using   std::string,
            std::string_view,
            std::vector;

    using namespace kickstart::failure_handling;    // hopefully, fail
    using namespace kickstart::language;            // Type_, end_ptr_of

    class Commandline
    {
        Commandline( const Commandline& ) = delete;
        auto operator=( const Commandline& ) -> Commandline& = delete;

    protected:
        string                  m_command_line;
        vector<string>          m_parts;
        vector<string_view>     m_part_views;

        Commandline() {}        // Makes the class effectively abstract.

    public:
        auto fulltext() const -> string_view    { return m_command_line; }
        auto verb() const -> string_view        { return m_part_views[0]; }

        auto args() const
            -> Array_span_<const string_view>
        { return {begin_ptr_of( m_part_views ) + 1, end_ptr_of( m_part_views )}; }

        operator string_view() const { return fulltext(); }

        static inline auto singleton() -> const Commandline&;
    };

    inline auto commandline()
        -> const Commandline&
    { return Commandline::singleton(); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Commandline,
        d::commandline;
    }  // namespace exported_names
}  // namespace kickstart::process::_definitions

namespace kickstart::process        { using namespace _definitions; }
