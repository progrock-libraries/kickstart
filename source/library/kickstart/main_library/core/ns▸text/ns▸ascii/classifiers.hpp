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

#include <kickstart/main_library/core/ns▸text/ns▸ascii/names.hpp>
#include <kickstart/main_library/core/ns▸language/types/Truth.hpp>

#include <string_view>

namespace kickstart::text::ascii {
    using language::Truth;
    using std::string_view;

    using Uchar = unsigned char;

    constexpr auto is_in( const string_view& chars, const char ch )
        -> Truth
    { return chars.find( ch ) != string_view::npos; }

    constexpr auto is_valid( char ch ) -> Truth         { return (Uchar( ch ) < n_codes); }
    constexpr auto is_control( char ch ) -> Truth       { return (Uchar( ch ) < n_low_control_codes or ch == del); }
    constexpr auto is_character( char ch ) -> Truth     { return is_valid( ch ); }
    constexpr auto is_lowercase( char ch ) -> Truth     { return ('a' <= ch and ch <= 'z'); }
    constexpr auto is_uppercase( char ch ) -> Truth     { return ('A' <= ch and ch <= 'Z'); }
    constexpr auto is_letter( char ch ) -> Truth        { return (is_lowercase( ch ) or is_uppercase( ch )); }
    constexpr auto is_digit( char ch ) -> Truth         { return ('0' <= ch and ch <= '9'); }

    constexpr auto is_uppercase_hex_digit_letter( char ch ) -> Truth { return ('A' <= ch and ch <= 'F'); }
    constexpr auto is_lowercase_hex_digit_letter( char ch ) -> Truth { return ('a' <= ch and ch <= 'f'); }

    constexpr auto is_hex_digit_letter( char ch )
        -> Truth
    { return (is_uppercase_hex_digit_letter( ch ) or is_lowercase_hex_digit_letter( ch )); }

    constexpr auto is_hex_digit( char ch ) -> Truth     { return (is_digit( ch ) or is_hex_digit_letter( ch )); }
    constexpr auto is_whitespace( char ch ) -> Truth    { return is_in( " \f\n\r\t\v", ch ); }


    //----------------------------------------------------------------------------------------------------
    // Alternate invocation syntax, may be more readable, may possibly support certain template constructs.

    template< auto (*f)( char ch ) -> Truth >
    struct Group_
    {
        static constexpr auto contains( const char ch )
            -> Truth
        { return f( ch ); }
    };

    using Valid                         = Group_<is_valid>;
    using Control                       = Group_<is_control>;
    using Lowercase                     = Group_<is_lowercase>;
    using Uppercase                     = Group_<is_uppercase>;
    using Letter                        = Group_<is_letter>;
    using Digit                         = Group_<is_digit>;
    using Uppercase_hex_digit_letter    = Group_<is_uppercase_hex_digit_letter>;
    using Lowercase_hex_digit_letter    = Group_<is_lowercase_hex_digit_letter>;
    using Hex_digit_letter              = Group_<is_hex_digit_letter>;
    using Hex_digit                     = Group_<is_hex_digit>;
    using Whitespace                    = Group_<is_whitespace>;

    template< class Group >
    constexpr auto is_( const char ch )
        -> Truth
    { return Group::contains( ch ); }

    template< class Group >
    constexpr auto is_all_( const string_view& s )
        -> Truth
    {
        for( const char ch : s ) {
            if( not is_<Group>( ch ) ) {
                return false;
            }
        }
        return true;
    }
}  // namespace kickstart::text::ascii
