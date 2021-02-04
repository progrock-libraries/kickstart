// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../assertion-headers/~assert-reasonable-compiler.hpp"

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

#include "../language/collection-util.hpp"              // int_size
#include "../language/stdlib-extensions/strings.hpp"    // spaces
#include "character-util.hpp"

#include <string>
#include <string_view>

namespace kickstart::ascii::_definitions {
    using namespace kickstart::language;        // begin_ptr_of, end_ptr_of, int_size, spaces
    using namespace std::string_view_literals;  // ""sv
    using   std::string,
            std::string_view;

    inline auto is_all_ascii( const string_view& s )
        -> bool
    {
        for( const char ch: s ) {
            if( not is( ascii::character, ch ) ) {
                return false;
            }
        }
        return true;
    }

    inline auto trimmed_view( const string_view& s )
        -> string_view
    {
        const char* p_first     = begin_ptr_of( s );
        const char* p_beyond    = end_ptr_of( s );

        while( p_first != p_beyond and is( ascii::space, *p_first ) ) {
            ++p_first;
        }
        while( p_beyond != p_first and is( ascii::space, *(p_beyond - 1) ) ) {
            --p_beyond;
        }

        return {p_first, static_cast<size_t>( p_beyond - p_first )};
    }

    inline auto trimmed_string( const string_view& s )
        -> string
    { return string( trimmed_view( s ) ); }

    inline auto trimmed( const string_view& s )
        -> string_view
    { return trimmed_view( s ); }

    inline auto trimmed( const string& s )
        -> string
    { return trimmed_string( s ); }

    inline auto at_left_in( const int field_width, const string_view& s )
        -> string
    { return string( s ) + spaces( field_width - int_size( s ) ); }

    inline auto at_right_in( const int field_width, const string_view& s )
        -> string
    { return spaces( field_width - int_size( s ) ) + string( s ); }

    inline auto to_lower( const string_view& s )
        -> string
    {
        string result;
        result.reserve( s.length() );
        for( const char ch: s ) {
            result += ascii::to_lower( ch );
        }
        return result;
    }

    inline auto to_upper( const string_view& s )
        -> string
    {
        string result;
        result.reserve( s.length() );
        for( const char ch: s ) {
            result += ascii::to_upper( ch );
        }
        return result;
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::is_all_ascii,
        d::trimmed_view, d::trimmed_string, d::trimmed,
        d::at_left_in, d::at_right_in,
        d::to_lower, d::to_upper;
    }  // namespace exported names
}  // namespace kickstart::ascii::_definitions

namespace kickstart::ascii  { using namespace kickstart::ascii::_definitions::exported_names; }
namespace kickstart::core   { namespace ascii = kickstart::ascii; }
