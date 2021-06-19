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

#include <kickstart/core/collection-util.hpp>
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/core/matrices/Abstract_matrix_.hpp>
#include <kickstart/core/matrices/vector-pool.hpp>

#include <kickstart/c/assert.hpp>

#include <algorithm>
#include <initializer_list>
#include <utility>
#include <vector>

namespace kickstart::matrices::_definitions {
    using kickstart::language::Truth;
    using   std::copy, std::swap,
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
        vector<Item>        m_items;
        two_d_grid::Size    m_size;

    public:
        ~Matrix_() { deallocate_vector( m_items ); }

        Matrix_( const two_d_grid::Size size = {} ):
            m_items( allocate_vector_<Item>( size.w*size.h ) ),
            m_size( size )
        {}

        Matrix_( const int width, const int height ):
            Matrix_( two_d_grid::Size{ width, height } )
        {}

        Matrix_( const int size ):
            Matrix_( two_d_grid::Size{ size, size} )
        {}

        Matrix_( const two_d_grid::Size size, const initializer_list<initializer_list<Item>>& values ):
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
            Matrix_( two_d_grid::Size{ int_size( values ), int_size( *values.begin() ) }, values )
        {}

        Matrix_( const Matrix_& other ):
            m_items( allocate_vector_<Item>( other.m_items.size() ), false ),
            m_size( other.m_size )
        {
            m_items = other.m_items;
        }

        Matrix_( Matrix_&& other ):
            m_items( move( other.m_items ) ),
            m_size( other.m_size )
        {}

        //! std::vector can't have const items.
        //operator Matrix_<const Item>& () const
        //{
        //    return reinterpret_cast<Matrix_<const Item>&>( *this );
        //}

        auto size() const       -> two_d_grid::Size { return m_size; }
        auto width() const      -> int              { return m_size.w; }
        auto height() const     -> int              { return m_size.h; }

        auto items_index_for( const two_d_grid::Position& pos ) const
            -> int
        { return pos.y*m_size.w + pos.x; }

        auto p_items()          -> Item*        { return m_items.data(); }
        auto p_items() const    -> const Item*  { return m_items.data(); }
        auto n_items() const    -> int          { return m_size.w*m_size.h; }

        auto operator()( const two_d_grid::Position& pos )
            -> Item&
        { return m_items[items_index_for( pos )]; }

        auto operator()( const two_d_grid::Position& pos ) const
            -> const Item&
        { return m_items[items_index_for( pos )]; }

        auto operator()( const int x, const int y )
            -> Item&
        { return (*this)( {x, y} ); }

        auto operator()( const int x, const int y ) const
            -> const Item&
        { return (*this)( {x, y} ); }
    };

    template< class Item >
    void swap_rows( const int i1, const int i2, Matrix_<Item>& m )
    {
        if( i1 == i2 ) { return; }

        auto p1 = m.p_items() + m.items_index_for({ 0, i1 });
        auto p2 = m.p_items() + m.items_index_for({ 0, i2 });

        for( int x = 0, w = m.width(); x < w; ++x ) {
            swap( *p1++, *p2++ );
        }
    }

    template< class Item >
    void swap_columns( const int i1, const int i2, Matrix_<Item>& m )
    {
        if( i1 == i2 ) { return; }

        const two_d_grid::Size size = m.size();
        if( size.w == 0 ) { return; }

        auto p1 = m.p_items() + m.items_index_for({ i1, 0 });
        auto p2 = m.p_items() + m.items_index_for({ i2, 0 });

        for( int count = 1; ; ++count ) {
            swap( *p1, *p2 );
            if( count == size.h ) { break; }
            p1 += size.w;  p2 += size.w;
        }
    }

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Abstract_matrix_ref_,
        d::Matrix_,
        d::swap_rows,
        d::swap_columns;
    }  // namespace exported names
}  // namespace kickstart::matrices::_definitions

namespace kickstart::matrices   { using namespace _definitions::exported_names;}
