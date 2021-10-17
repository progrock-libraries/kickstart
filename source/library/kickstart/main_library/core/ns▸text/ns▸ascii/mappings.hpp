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

#include <kickstart/main_library/core/ns▸text/ns▸ascii/classifiers.hpp>

#include <kickstart/main_library/core/ns▸language/types/Truth.hpp>
#include <string_view>

namespace kickstart::text::ascii {
    using language::Truth;
    using std::string_view;

    inline auto lowercased( const char ch )
        -> char
    { return (is_uppercase( ch )? char( ch - 'A' + 'a' ) : ch); }

    inline auto uppercased( const char ch )
        -> char
    { return (is_lowercase( ch )? char( ch - 'a' + 'A' ) : ch); }

    inline auto basic_digit_char_from( const int digit )
        -> char
    { return "0123456789"[digit]; }

    inline auto basic_hex_digit_char_from( const int digit )
        -> char
    { return "0123456789ABCDEF"[digit]; }

    inline auto basic_to_digit( const char ch )
        -> int
    { return ch - '0'; }

    inline auto digit_char_from( const int digit )
        -> char
    { return (unsigned( digit ) < 10? basic_digit_char_from( digit ) : '\0'); }

    inline auto hex_digit_char_from( const int digit )
        -> char
    { return (unsigned( digit ) < 16? basic_hex_digit_char_from( digit ) : '\0'); }

    inline auto to_digit( const char ch )
        -> int
    { return (is_digit( ch )? basic_to_digit( ch ) : '\0'); }

    inline auto to_hex_digit( const char ch )
        -> int
    {
        return (0?0
            : is_digit( ch )?                       ch - '0'
            : is_uppercase_hex_digit_letter( ch )?  10 + (ch - 'A')
            : is_lowercase_hex_digit_letter( ch )?  10 + (ch - 'a')
            :                                       -1
            );
    }

}  // namespace kickstart::ascii
