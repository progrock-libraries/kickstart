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

#include <kickstart/core/collection-util.hpp>
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/core/matrices/Abstract_matrix_.hpp>
#include <kickstart/core/matrices/vector-pool.hpp>

#include <unordered_map>
#include <vector>
#include <utility>

namespace kickstart::matrices::_definitions {
    using kickstart::core::Truth;

    template< class Item_type_param >
    class Abstract_matrix_ref_;

    template< class Item_type_param >
    class Matrix_
    {
    public:
        using Item = Item_type_param;

    private:
        vector<Item>    m_items;
        Size            m_size;

    public:
        ~Matrix_() { deallocate_vector( m_items ); }

        Matrix_( const Size size = {} ):
            m_items( allocate_vector_<Item>( size.w*size.h ) ),
            m_size( size )
        {}

        Matrix_( const Matrix_& other ):
            m_items( allocate_vector_<Item>( other.m_items.size() ), false ),
            m_size( other.m_size )
        {
            m_items = other.m_items;
        }

        Matrix_( Matrix_&& other ):
            m_items( move( other ) ),
            m_size( other.m_size )
        {}

        operator Matrix_<const Item>& () const
        {
            return reinterpret_cast<Matrix_<const Item>&>( *this );
        }

        inline auto abstract_ref() -> Abstract_matrix_ref_<Item>;
        inline auto abstract_ref() const -> Abstract_matrix_ref_<const Item>;

        auto size() const       -> Size { return m_size; }
        auto width() const      -> int  { return m_size.w; }
        auto height() const     -> int  { return m_size.h; }

        auto items_index_for( const Position& pos ) const
            -> int
        {
            const int w = m_size.w;
            return pos.y*w + pos.x;
        }

        auto items()        -> Item*        { return m_items.data(); }
        auto items() const  -> const Item*  { return m_items.data(); }

        auto operator()( const Position& pos )
            -> Item&
        { return m_items[items_index_for( pos )]; }

        auto operator()( const Position& pos ) const
            -> const Item&
        { return m_items[items_index_for( pos )]; }
    };

    template< class Item_type_param >
    class Abstract_matrix_ref_:
        public Abstract_matrix_<Item_type_param>
    {
    public:
        using Item = Item_type_param;

    private:
        Matrix_<Item_type_param>*   m_p_matrix;

    public:
        Abstract_matrix_ref_( Matrix_<Item>& m ):
            m_p_matrix( &m )
        {}

        operator Abstract_matrix_ref_<const Item>& () const
        {
            return reinterpret_cast<Abstract_matrix_ref_<const Item>&>( *this );
        }

        virtual auto size() const   -> Size         { return m_p_matrix->size(); }
        virtual auto items()        -> Item*        { return m_p_matrix->items(); }
        virtual auto items() const  -> const Item*  { return m_p_matrix->items(); }
    };

    template< class Item_type_param >
    auto Matrix_<Item_type_param>::abstract_ref()
        -> Abstract_matrix_ref_<Item_type_param>
    {
        return Abstract_matrix_ref_<Item_type_param>( *this );
    }

    template< class Item_type_param >
    auto Matrix_<Item_type_param>::abstract_ref() const
        -> Abstract_matrix_ref_<const Item_type_param>
    {
        return Abstract_matrix_ref_<const Item_type_param>(
            static_cast<Matrix_<const Item>&>( *this )
            );
    }

}  // namespace kickstart::matrices::_definitions
