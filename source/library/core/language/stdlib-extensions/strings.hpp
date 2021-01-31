﻿// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../../assertion-headers/~assert-reasonable-compiler.hpp"

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

//#include "character-util.hpp"
#include "../../language/collection-util.hpp"   // int_size

#include <string>
#include <string_view>

namespace kickstart::language::_definitions {
    using namespace std::string_view_literals;  // ""sv
    using   std::string,
            std::string_view;

    inline auto repeated_times( const int n, const string_view& s )
        -> string
    {
        string result;
        for( int i = 1; i <= n; ++i ) {
            result += s;
        }
        return result;
    }

    inline auto operator*( const int n, const string_view& s )
    -> string
    { return repeated_times( n, s ); }

    inline auto spaces( const int n )
        -> string
    { return n * " "sv; }

    inline auto at_left_in( const int field_width, const string_view& s )
        -> string
    { return string( s ) + spaces( field_width - int_size( s ) ); }

    inline auto at_right_in( const int field_width, const string_view& s )
        -> string
    { return spaces( field_width - int_size( s ) ) + string( s ); }

    template< class... Args >
    inline auto concatenated( const Args&... args )
        -> string
    { return (std::string() << ... << args); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::repeated_times, d::operator*,
        d::spaces,
        d::at_left_in, d::at_right_in,
        d::concatenated;
    }  // namespace exported names
}  // namespace kickstart::language::_definitions

namespace kickstart::language       { using namespace _definitions::exported_names; }
namespace kickstart::core           { using namespace language; }