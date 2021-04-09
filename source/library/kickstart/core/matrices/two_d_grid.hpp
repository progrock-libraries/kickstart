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

namespace kickstart::two_d_grid::_definitions {
    struct Position     { int x; int y; };
    struct Size         { int w; int h; };

    inline auto operator+( const Position& pos, const Size& offset )
        -> Position
    { return Position{ pos.x + offset.w, pos.y + offset.h }; }

    inline auto operator-( const Position& pos, const Size& offset )
        -> Position
    { return Position{ pos.x - offset.w, pos.y - offset.h }; }

    inline auto operator-( const Position& a, const Position& b )
        -> Size
    { return Size{ a.x - b.x, a.y - b.y }; }

    inline void operator+=( Position& pos, const Size& offset )
    {
        pos = pos + offset;
    }

    inline void operator-=( Position& pos, const Size& offset )
    {
        pos = pos - offset;
    }

    inline auto operator+( const Size& a, const Size& b )
        -> Size
    { return Size{ a.w + b.w, a.h + b.h }; }


    inline auto operator-( const Size& a, const Size& b )
        -> Size
    { return Size{ a.w - b.w, a.h - b.h }; }

    inline void operator+=( Size& size, const Size& delta )
    {
        size = size + delta;
    }

    inline void operator-=( Size& size, const Size& delta )
    {
        size = size - delta;
    }

    inline auto rotated_left( const Size& size )
        -> Size
    { return Size{ size.h, -size.w }; }

    inline auto rotated_right( const Size& size )
        -> Size
    { return Size{ -size.h, size.w }; }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Position, d::Size,
        d::operator+, d::operator-,
        d::operator+=, d::operator-=,
        d::rotated_left, d::rotated_right;
    }
}  // namespace kickstart::two_d_grid::_definitions

namespace kickstart::two_d_grid { using namespace _definitions::exported_names; }
