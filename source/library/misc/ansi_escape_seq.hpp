// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once

// kickstart.core.hpp - minimal convenience functionality for C++ learners.
// The “core” is because the (rather large) iostreams headers are not included.
//
// Requires C++17 or later.

// Copyright (c) 2020 Alf P. Steinbach. MIT license, with license text:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "../core/text_conversion/to-text.hpp"

namespace kickstart::ansi_escape_seq::_definitions {
    using namespace kickstart::text_conversion;

    using   std::string;

    inline const char       escape  = char( 27 );
    inline const string     lead_in = ""s << escape << '[';

    inline auto color( const int n )
        -> string
    { return ""s << lead_in << n << "m"; }

    namespace colors {
        inline const string restore     = color( 0 );
        inline const string red         = color( 91 );
        inline const string yellow      = color( 93 );
        inline const string blue        = color( 96 );
    }  // namespace colors

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::escape,
        d::lead_in,
        d::color;
    }  // namespace exported names
}  // namespace kickstart::ansi_escape_seq::_definitions

namespace kickstart::ansi_escape_seq {
    using namespace _definitions::exported_names;
    inline namespace colors { using namespace _definitions::colors; }
}
