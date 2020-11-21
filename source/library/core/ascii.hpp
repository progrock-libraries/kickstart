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

namespace kickstart::ascii {

    template< class Group >
    constexpr auto is_( Group, const char ch )
        -> bool
    { return Group::includes( ch ); }

    struct {
        static constexpr auto includes( const char ch )
            -> bool
        { return (0 <= ch and ch < 128); }
    } constexpr character;

    struct {
        static constexpr auto includes( const char ch )
            -> bool
        { return ('a' <= ch and ch <= 'z'); }
    } constexpr lowercase;

    struct {
        static constexpr auto includes( const char ch )
            -> bool
        { return ('A' <= ch and ch <= 'B'); }
    } constexpr uppercase;

    struct {
        static constexpr auto includes( const char ch )
            -> bool
        { return (is_( lowercase, ch ) or is_( uppercase, ch )); }
    } constexpr letter;

    struct {
        static constexpr auto includes( const char ch )
            -> bool
        { return ('0' <= ch and ch <= '9'); }
    } constexpr digit;

    struct {
        static constexpr auto includes( const char ch )
            -> bool
        { return (ch == ' ' or ch == '\f' or ch == '\n' or ch == '\r' or ch == '\t' or ch == '\v'); }
    } constexpr space;

    inline auto ascii_to_lower( const char ch )
        -> char
    { return (is_( uppercase, ch )? char( ch - 'A' + 'a' ) : ch); }

    inline auto ascii_to_upper( const char ch )
        -> char
    { return (is_( lowercase, ch )? char( ch - 'a' + 'A' ) : ch); }

}  // namespace kickstart::ascii

namespace kickstart::all {
    namespace ascii = kickstart::ascii;
}  // namespace kickstart::all
