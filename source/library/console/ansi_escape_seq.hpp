// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../assertion-headers/assert-reasonable-compiler.hpp"

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

    inline auto home()
        -> string
    { return lead_in + "H"; }

    inline auto cursor_to( const int column, const int line )
        -> string
    { return ""s << lead_in << line << ";" << column << "H"; }

    inline auto clear_console()
        -> string
    { return home() << lead_in << "2J"; }

    inline auto color( const int n )
        -> string
    { return ""s << lead_in << n << "m"; }

    namespace colors {
        // https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#text-formatting

        inline const string restore             = color( 0 );
        inline const string bold                = color( 1 );
        inline const string underline           = color( 4 );
        inline const string no_underline        = color( 24 );
        inline const string negative            = color( 7 );
        inline const string positive            = color( 27 );

        inline const string black               = color( 30 );
        inline const string red                 = color( 31 );
        inline const string green               = color( 32 );
        inline const string yellow              = color( 33 );
        inline const string blue                = color( 34 );
        inline const string magenta             = color( 35 );
        inline const string cyan                = color( 36 );
        inline const string white               = color( 37 );
        inline const string extended            = color( 38 );
        inline const string restore_fg          = color( 39 );

        inline const string bg_black            = color( 40 );
        inline const string bg_red              = color( 41 );
        inline const string bg_green            = color( 42 );
        inline const string bg_yellow           = color( 43 );
        inline const string bg_blue             = color( 44 );
        inline const string bg_magenta          = color( 45 );
        inline const string bg_cyan             = color( 46 );
        inline const string bg_white            = color( 47 );
        inline const string bg_extended         = color( 48 );
        inline const string restore_bg          = color( 49 );

        inline const string bright_black        = color( 90 );
        inline const string bright_red          = color( 91 );
        inline const string bright_green        = color( 92 );
        inline const string bright_yellow       = color( 93 );
        inline const string bright_blue         = color( 94 );
        inline const string bright_magenta      = color( 95 );
        inline const string bright_cyan         = color( 96 );
        inline const string bright_white        = color( 97 );
        inline const string bg_bright_black     = color( 100 );
        inline const string bg_bright_red       = color( 101 );
        inline const string bg_bright_green     = color( 102 );
        inline const string bg_bright_yellow    = color( 103 );
        inline const string bg_bright_blue      = color( 104 );
        inline const string bg_bright_magenta   = color( 105 );
        inline const string bg_bright_white     = color( 106 );
    }  // namespace colors

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::escape,
        d::lead_in,
        d::home,
        d::cursor_to,
        d::clear_console,
        d::color;
    }  // namespace exported names
}  // namespace kickstart::ansi_escape_seq::_definitions

namespace kickstart::ansi_escape_seq {
    using namespace _definitions::exported_names;
    inline namespace colors { using namespace _definitions::colors; }
}
