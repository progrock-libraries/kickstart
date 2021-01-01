// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../assertion-headers/assert-reasonable-compiler.hpp"

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

// These symbols are portable in the sense that they can be presented in Windows.
namespace kickstart::portable_dingbats {
    struct Codes
    {
        char        pc;         // Windows codepage 437.
        char        utf8[6];    // Unicode.
        operator const char* () const { return utf8; }
    };

    constexpr auto smiley               = Codes{ char(  1 ), "☺" };
    constexpr auto inv_smiley           = Codes{ char(  2 ), "☻" };
    constexpr auto suit_heart           = Codes{ char(  3 ), "♥" };
    constexpr auto suit_diamonds        = Codes{ char(  4 ), "♦" };
    constexpr auto suit_clubs           = Codes{ char(  5 ), "♣" };
    constexpr auto suit_spades          = Codes{ char(  6 ), "♠" };
    constexpr auto bullet_point         = Codes{ char(  7 ), "•" };
    constexpr auto inv_bullet_point     = Codes{ char(  8 ), "◘" };
    constexpr auto circle               = Codes{ char(  9 ), "○" };
    constexpr auto inv_circle           = Codes{ char( 10 ), "◙" };
    constexpr auto gender_male          = Codes{ char( 11 ), "♂" };
    constexpr auto gender_female        = Codes{ char( 12 ), "♀" };
    constexpr auto musical_note         = Codes{ char( 13 ), "♪" };
    constexpr auto musical_double_note  = Codes{ char( 14 ), "♫" };
    constexpr auto sun                  = Codes{ char( 15 ), "☼" };
    constexpr auto pointer_right        = Codes{ char( 16 ), "►" };
    constexpr auto pointer_left         = Codes{ char( 17 ), "◄" };
    constexpr auto arrow_up_down        = Codes{ char( 18 ), "↕" };
    constexpr auto double_exclamation   = Codes{ char( 19 ), "‼" };
    constexpr auto paragraph            = Codes{ char( 20 ), "¶" };
    constexpr auto section_sign         = Codes{ char( 21 ), "§" };
    constexpr auto bold_m_dash          = Codes{ char( 22 ), "▬" };
    constexpr auto arrow_up_down_based  = Codes{ char( 23 ), "↨" };
    constexpr auto arrow_up             = Codes{ char( 24 ), "↑" };
    constexpr auto arrow_down           = Codes{ char( 25 ), "↓" };
    constexpr auto arrow_right          = Codes{ char( 26 ), "→" };
    constexpr auto arrow_left           = Codes{ char( 27 ), "←" };
    constexpr auto right_angle          = Codes{ char( 28 ), "∟" };
    constexpr auto arrow_left_right     = Codes{ char( 29 ), "↔" };
    constexpr auto pointer_up           = Codes{ char( 30 ), "▲" };
    constexpr auto pointer_down         = Codes{ char( 31 ), "▼" };
}  // namespace kickstart::portable_dingbats
