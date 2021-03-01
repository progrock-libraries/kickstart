// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/core/language/assertion-headers/~assert-reasonable-compiler.hpp>

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

#include <kickstart/core/language/Truth.hpp>

namespace kickstart::ascii {
    using language::Truth;

    template< class Group >
    constexpr auto is( Group, const char ch )
        -> Truth
    { return Group::includes( ch ); }

    struct Character
    {
        static constexpr auto includes( const char ch )
            -> Truth
        { return (static_cast<unsigned char>( ch ) < 128); }
    };
    constexpr Character     character   = {};

    struct Lowercase
    {
        static constexpr auto includes( const char ch )
            -> Truth
        { return ('a' <= ch and ch <= 'z'); }
    };
    constexpr Lowercase     lowercase   = {};

    struct Uppercase
    {
        static constexpr auto includes( const char ch )
            -> Truth
        { return ('A' <= ch and ch <= 'B'); }
    };
    constexpr Uppercase     uppercase   = {};

    struct Letter
    {
        static constexpr auto includes( const char ch )
            -> Truth
        { return (is( lowercase, ch ) or is( uppercase, ch )); }
    };
    constexpr Letter        letter      = {};

    struct Digit
    {
        static constexpr auto includes( const char ch )
            -> Truth
        { return ('0' <= ch and ch <= '9'); }
    };
    constexpr Digit         digit       = {};

    struct Whitespace
    {
        static constexpr auto includes( const char ch )
            -> Truth
        { return (ch == ' ' or ch == '\f' or ch == '\n' or ch == '\r' or ch == '\t' or ch == '\v'); }
    };
    constexpr Whitespace    whitespace  = {};

    inline auto to_lower( const char ch )
        -> char
    { return (is( uppercase, ch )? char( ch - 'A' + 'a' ) : ch); }

    inline auto to_upper( const char ch )
        -> char
    { return (is( lowercase, ch )? char( ch - 'a' + 'A' ) : ch); }

}  // namespace kickstart::ascii
