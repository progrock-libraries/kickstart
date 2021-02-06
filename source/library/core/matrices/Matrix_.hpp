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

#include <kickstart/core/collection-util.hpp>
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/core/matrices/Abstract_matrix_.hpp>
#include <kickstart/core/matrices/vector-pool.hpp>

#include <assert.h>

#include <algorithm>
#include <initializer_list>
#include <utility>
#include <vector>

namespace kickstart::matrices::_definitions {
    using kickstart::core::Truth;
    using   std::copy,
            std::initializer_list,
            std::move,
            std::vector;

    template< class Item_type_param >
    class Abstract_matrix_ref_;

    template< class Item_type_param >
    class Matrix_
    {
    public:
        using Item = Item_type_param;

    private:
        vector<Item>    m_items;
        Two_d_size      m_size;

    public:
        ~Matrix_() { deallocate_vector( m_items ); }

        Matrix_( const Two_d_size size = {} ):
            m_items( allocate_vector_<Item>( size.w*size.h ) ),
            m_size( size )
        {}

        Matrix_( const Two_d_size size, const initializer_list<initializer_list<Item>>& values ):
            m_items( allocate_vector_<Item>( size.h * size.w ) ),
            m_size( size )
        {
            const int first_row_size = int_size( *values.begin() );

            assert( int_size( values ) == m_size.h );
            assert( first_row_size == m_size.w );
            Item* p_row = m_items.data();
            for( const initializer_list<Item>& row: values ) {
                assert( int_size( row ) == first_row_size );
                copy( row.begin(), row.end(), p_row );
                p_row += m_size.w;
            }
        }

        Matrix_( const initializer_list<initializer_list<Item>>& values ):
            Matrix_( Two_d_size{ int_size( values ), int_size( *values.begin() ) }, values )
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

        auto size() const       -> Two_d_size   { return m_size; }
        auto width() const      -> int          { return m_size.w; }
        auto height() const     -> int          { return m_size.h; }

        auto items_index_for( const Two_d_position& pos ) const
            -> int
        {
            const int w = m_size.w;
            return pos.y*w + pos.x;
        }

        auto items()        -> Item*        { return m_items.data(); }
        auto items() const  -> const Item*  { return m_items.data(); }

        auto operator()( const Two_d_position& pos )
            -> Item&
        { return m_items[items_index_for( pos )]; }

        auto operator()( const Two_d_position& pos ) const
            -> const Item&
        { return m_items[items_index_for( pos )]; }

        auto operator()( const int x, const int y )
            -> Item&
        { return operator()( {x, y} ); }

        auto operator()( const int x, const int y ) const
            -> const Item&
        { return operator()( {x, y} ); }
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

        virtual auto size() const   -> Two_d_size   { return m_p_matrix->size(); }
        virtual auto items()        -> Item*        { return m_p_matrix->items(); }
        virtual auto items() const  -> const Item*  { return m_p_matrix->items(); }
    };

    template< class Item_type_param >
    auto Matrix_<Item_type_param>::abstract_ref()
        -> Abstract_matrix_ref_<Item_type_param>
    { return Abstract_matrix_ref_<Item_type_param>( *this ); }

    template< class Item_type_param >
    auto Matrix_<Item_type_param>::abstract_ref() const
        -> Abstract_matrix_ref_<const Item_type_param>
    {
        auto& const_item_m = static_cast<Matrix_<const Item>&>( *this );
        return Abstract_matrix_ref_<const Item_type_param>( const_item_m );
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Abstract_matrix_ref_,
        d::Matrix_;
    }  // namespace exported names
}  // namespace kickstart::matrices::_definitions

namespace kickstart::matrices   { using namespace _definitions::exported_names;}
namespace kickstart::core       { using namespace matrices; }
