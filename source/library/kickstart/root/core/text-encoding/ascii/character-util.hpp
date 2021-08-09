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

#include <kickstart/root/core/language/types/Truth.hpp>
#include <string_view>

namespace kickstart::ascii {
    using language::Truth;
    using std::string_view;

    inline auto is_in( const string_view& chars, const char ch )
        -> Truth
    { return chars.find( ch ) != string_view::npos; }

    inline auto is_valid( char ch ) -> Truth        { return (static_cast<unsigned char>( ch ) < 128); }
    inline auto is_character( char ch ) -> Truth    { return is_valid( ch ); }
    inline auto is_lowercase( char ch ) -> Truth    { return ('a' <= ch and ch <= 'z'); }
    inline auto is_uppercase( char ch ) -> Truth    { return ('A' <= ch and ch <= 'Z'); }
    inline auto is_letter( char ch ) -> Truth       { return (is_lowercase( ch ) or is_uppercase( ch )); }
    inline auto is_digit( char ch ) -> Truth        { return ('0' <= ch and ch <= '9'); }
    inline auto is_whitespace( char ch ) -> Truth   { return is_in( " \f\n\r\t\v", ch ); }

    inline auto to_lower( const char ch )
        -> char
    { return (is_uppercase( ch )? char( ch - 'A' + 'a' ) : ch); }

    inline auto to_upper( const char ch )
        -> char
    { return (is_lowercase( ch )? char( ch - 'a' + 'A' ) : ch); }

    // Alternate invocation syntax, may possibly support certain template construcs.
    template< class Group >
    constexpr auto is( Group, const char ch )
        -> Truth
    { return Group::includes( ch ); }

    template< auto (*f)( char ch ) -> Truth >
    struct Group_
    {
        static constexpr auto includes( const char ch )
            -> Truth
        { return f( ch ); }
    };

    constexpr auto valid        = Group_<is_valid>();
    constexpr auto character    = valid;
    constexpr auto lowercase    = Group_<is_lowercase>();
    constexpr auto uppercase    = Group_<is_uppercase>();
    constexpr auto letter       = Group_<is_letter>();
    constexpr auto digit        = Group_<is_digit>();
    constexpr auto whitespace   = Group_<is_whitespace>();

}  // namespace kickstart::ascii
