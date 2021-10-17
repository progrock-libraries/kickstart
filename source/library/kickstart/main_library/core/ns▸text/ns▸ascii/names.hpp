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

#include <kickstart/main_library/core/ns▸language/syntax-support/KS_STATIC_ASSERT.hpp>

namespace kickstart::text::ascii {
    // Standard but unfortunately uppercase abbreviations as per ASCII '67:
    constexpr char      NUL         = '\0';         
    constexpr char      BEL         = '\a';         
    constexpr char      BS          = '\b';         
    constexpr char      HT          = '\t';         // “Horizontal tab”
    constexpr char      LF          = '\n';         
    constexpr char      VT          = '\v';         // “Vertical tab”
    constexpr char      FF          = '\f';         
    constexpr char      CR          = '\r';         // “Carriage return”
    constexpr char      DC1         = '\x11';       // “Device control 1”
    constexpr char      DC3         = '\x13';       // “Device control 3”
    constexpr char      SUB         = '\x1A';       // “Substitute”, but see below!
    constexpr char      ESC         = '\x1B';       // “Escape”, e.g. start of ANSI sequences.

    constexpr char      SPC         = ' ';          

    constexpr char      DEL         = '\x7F';       // “Delete”

    // More readable & self-describing names:
    constexpr char      null        = NUL;          // '\0' 0x00
    constexpr char      bell        = BEL;          // '\a' 0x07, ^G
    constexpr char      backspace   = BS;           // '\b' 0x07, ^H
    constexpr char      tab         = HT;           // '\t' 0x08, ^I, horizontal tab
    constexpr char      linefeed    = LF;           // '\n' 0x09, ^J
    constexpr char      vtab        = VT;           // '\v' 0x0A, ^K
    constexpr char      formfeed    = FF;           // '\f' 0x0B, ^L
    constexpr char      enter       = CR;           // '\r' 0x0C, ^M
    constexpr char      xon         = DC1;          //      0x11, ^Q, "continue"
    constexpr char      xoff        = DC3;          //      0x13, ^S, "stop"
    constexpr char      end_of_text = SUB;          //      0x1A, ^Z, non-std MS meaning.
    constexpr char      escape      = ESC;          //      0x1B
    constexpr char      space       = SPC;          //      0x20

    constexpr char      del         = DEL;          //      0x7F

    constexpr char      first_code  = char( 0 );    KS_STATIC_ASSERT( first_code == null );
    constexpr char      last_code   = char( 127 );  KS_STATIC_ASSERT( last_code == del );

    constexpr int       n_low_control_codes = 32;   KS_STATIC_ASSERT( n_low_control_codes == space );
    constexpr int       n_codes     = last_code + 1;

    // ASCII DEL, code 0x7F = 127, “delete”, can be used as a replacement for encoding
    // errors or unrepresentable code point.
    //
    // ASCII has a character dedicated to the purpose: SUB “substitute”, code 26,
    // ^Z. But in Windows ^Z is used as an EOF marker for text streams. So ^Z as
    // replacement is not usable in Windows, i.e. it's not a general solution.
    //
    // Unicode, an extension of ASCII, also has its own character dedicated to
    // the purpose, namely code U+FFFD “replacement character”. It translates to
    // at least two UTF-8 bytes so it's not trivial to check for. And it can't be
    // represented in simple single byte encodings such as codepage 437, so it's
    // not usable in single byte encodings, i.e. it's not a general solution.
    //
    // Using ASCII DEL as a replacement character does not preclude using it also to
    // represent the DEL key in a keyboard interface, for in that context there is no
    // possibility of encoding errors or unrepresentable code point. It doesn't appear
    // in ordinary text. And it even has some mnemonic value as a deleted character.
    //
    // For these reasons I recommend using ASCII DEL as replacement and error
    // indication character:
    //
    constexpr char      bad_char    = del;

}  // namespace kickstart::text::ascii
