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

#include <kickstart/core/text-conversion/to-text.hpp>

#include <unordered_map>

namespace kickstart::ansi_escape_seq::_definitions {
    using namespace kickstart::text_conversion;

    using   std::string,
            std::string_view,
            std::unordered_map;

    namespace impl {
        // The caching functions are not for efficiency but to support a C++17 interface
        // with only  `string_view` for the fixed strings, as if compile time constants.

        inline const char       escape  = char( 27 );
        inline const string     lead_in = ""s << escape << '[';

        namespace computed {
            inline auto color_string( const int n )
                -> string
            { return ""s << lead_in << n << "m"; }
        }  // namespace computed

        namespace cached {
            inline auto home_string()
                -> const string&
            {
                static const string s = ""s << lead_in << "H";
                return s;
            }

            inline auto clear_console_string()
                -> const string&
            {
                static const string s = ""s << home_string() << lead_in << "2J";
                return s;
            }

            inline auto color_string( const int n )
                -> const string&
            {
                static unordered_map<int, string>   strings;

                auto it = strings.find( n );
                if( it == strings.end() ) {
                    it = strings.insert( {n, computed::color_string( n )} ).first;
                }
                return it->second;
            }
        }  // namespace cached
    }

    inline const char           escape  = impl::escape;
    inline const string_view    lead_in = impl::lead_in;

    inline auto home()
        -> string_view
    { return impl::cached::home_string(); }

    inline auto cursor_to( const int column, const int line )
        -> string
    { return ""s << lead_in << line << ";" << column << "H"; }

    inline auto clear_console()
        -> string_view
    { return impl::cached::clear_console_string(); }

    inline auto set_console_title( const string_view& title )
        -> string
    { return ""s << escape << "]2;" << title << '\x07'; }

    inline auto color( const int n )
        -> string_view
    { return impl::cached::color_string( n ); }

    namespace colors {
        // https://docs.microsoft.com/en-us/windows/consoles/console-virtual-terminal-sequences#text-formatting

        inline const string_view restore            = color( 0 );
        inline const string_view bold               = color( 1 );
        inline const string_view underline          = color( 4 );
        inline const string_view no_underline       = color( 24 );
        inline const string_view negative           = color( 7 );
        inline const string_view positive           = color( 27 );

        inline const string_view black              = color( 30 );
        inline const string_view red                = color( 31 );
        inline const string_view green              = color( 32 );
        inline const string_view yellow             = color( 33 );
        inline const string_view blue               = color( 34 );
        inline const string_view magenta            = color( 35 );
        inline const string_view cyan               = color( 36 );
        inline const string_view white              = color( 37 );
        inline const string_view extended           = color( 38 );
        inline const string_view restore_fg         = color( 39 );

        inline const string_view bg_black           = color( 40 );
        inline const string_view bg_red             = color( 41 );
        inline const string_view bg_green           = color( 42 );
        inline const string_view bg_yellow          = color( 43 );
        inline const string_view bg_blue            = color( 44 );
        inline const string_view bg_magenta         = color( 45 );
        inline const string_view bg_cyan            = color( 46 );
        inline const string_view bg_white           = color( 47 );
        inline const string_view bg_extended        = color( 48 );
        inline const string_view restore_bg         = color( 49 );

        inline const string_view bright_black       = color( 90 );
        inline const string_view bright_red         = color( 91 );
        inline const string_view bright_green       = color( 92 );
        inline const string_view bright_yellow      = color( 93 );
        inline const string_view bright_blue        = color( 94 );
        inline const string_view bright_magenta     = color( 95 );
        inline const string_view bright_cyan        = color( 96 );
        inline const string_view bright_white       = color( 97 );
        inline const string_view bg_bright_black    = color( 100 );
        inline const string_view bg_bright_red      = color( 101 );
        inline const string_view bg_bright_green    = color( 102 );
        inline const string_view bg_bright_yellow   = color( 103 );
        inline const string_view bg_bright_blue     = color( 104 );
        inline const string_view bg_bright_magenta  = color( 105 );
        inline const string_view bg_bright_white    = color( 106 );
    }  // namespace colors

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::escape,
        d::lead_in,
        d::home,
        d::cursor_to,
        d::clear_console,
        d::set_console_title,
        d::color;
    }  // namespace exported names
}  // namespace kickstart::ansi_escape_seq::_definitions

namespace kickstart::ansi_escape_seq {
    using namespace _definitions::exported_names;
    inline namespace colors { using namespace _definitions::colors; }
}
