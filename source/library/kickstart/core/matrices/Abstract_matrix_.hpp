﻿// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
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

namespace kickstart::matrices::_definitions {
    using kickstart::language::Truth;

    namespace two_d_grid {
        struct Position     { int x; int y; };
        struct Size         { int w; int h; };

        inline auto operator+( const Position& pos, const Size& offset )
            -> Position
        { return Position{ pos.x + offset.w, pos.y + offset.h }; }

        inline auto operator-( const Position& pos, const Size& offset )
            -> Position
        { return Position{ pos.x - offset.w, pos.y - offset.h }; }

        inline void operator+=( Position& pos, const Size& offset )
        {
            pos = pos + offset;
        }

        inline void operator-=( Position& pos, const Size& offset )
        {
            pos = pos - offset;
        }
    }  // namespace two_d_grid

    template< class Item_type_param >
    class Abstract_matrix_
    {
    public:
        using Item = Item_type_param;

        virtual auto size() const       -> two_d_grid::Size     = 0;
        virtual auto p_items()          -> Item*                = 0;
        virtual auto p_items() const    -> const Item*          = 0;

        virtual auto width() const  -> int  { return size().w; }
        virtual auto height() const -> int  { return size().h; }

        auto items_index_for( const two_d_grid::Position& pos ) const
            -> int
        { return pos.y*width() + pos.x; }

        auto operator()( const two_d_grid::Position& pos )
            -> Item&
        { return p_items()[items_index_for( pos )]; }

        auto operator()( const two_d_grid::Position& pos ) const
            -> const Item&
        { return p_items()[items_index_for( pos )]; }
    };

    template< class Item, class Matrix >
    inline auto pointer_to_row( const int y, Matrix& m )
        -> auto*
    { return m.p_items() + m.item_index_for( {0, y} ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Abstract_matrix_,
        d::pointer_to_row;

        namespace two_d_grid = d::two_d_grid;
    }  // namespace exported names
}  // namespace kickstart::matrices::_definitions

namespace kickstart::matrices   { using namespace _definitions::exported_names;}
