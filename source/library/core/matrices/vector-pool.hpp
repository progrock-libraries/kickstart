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

#include <unordered_map>
#include <vector>
#include <utility>

namespace kickstart::matrices::_definitions {
    namespace kc = kickstart::core;
    using kc::Truth, kc::int_size;
    using   std::unordered_map,
            std::vector,
            std::swap;

    template< class Item_type_param >
    class Vector_pool_
    {
    public:
        using Item = Item_type_param;

    private:
        using Item_vector = vector<Item>;

        unordered_map<int, vector<Item_vector>> m_vectors;
        int                                     m_max_vector_size;
        int                                     m_max_capacity;

        Vector_pool_():
            m_max_vector_size( 4*4 ),
            m_max_capacity( 32 )
        {}

    public:
        using Item = Item_type_param;

        void remove_all() { decltype( m_vectors )().swap( m_vectors ); }

        auto allocate( const int size, const Truth zeroing = true )
            -> vector<Item>
        {
            if( size > m_max_vector_size ) {
                return vector<Item>( size );
            }
            vector<Item_vector>& vectors = m_vectors[size];
            if( vectors.empty() ) {
                vectors.reserve( m_max_capacity );
                return vector<Item>( size );
            } else {
                vector<Item> result;
                using std::swap;  swap( result, vectors.back() );
                vectors.pop_back();
                if( zeroing ) { for( Item& item: result ) { item = Item(); } }
                return result;
            }
        }

        void deallocate( vector<Item>& v )
        {
            using std::swap;
            const int size = int_size( v );
            if( size <= m_max_vector_size ) {
                vector<Item_vector>& vectors = m_vectors[size];
                vectors.push_back( move( v ) );
            }
            vector<Item>().swap( v );
        }

        static auto singleton()
            -> Vector_pool_&
        {
            static Vector_pool_ the_pool;
            return the_pool;
        }
    };

    template< class Item >
    inline auto allocate_vector_( const int size )
        -> vector<Item>
    { return Vector_pool_<Item>::singleton().allocate( size ); }

    template< class Item >
    inline void deallocate_vector( vector<Item>& v )
    {
        Vector_pool_<Item>::singleton().deallocate( v );
    }

}  // namespace kickstart::matrices::_definitions