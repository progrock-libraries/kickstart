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

#include <kickstart/main_library/core/ns▸collection_support/collection-pointers.hpp>
#include <kickstart/main_library/core/ns▸collection_support/collection-sizes.hpp>
#include <kickstart/main_library/core/failure-handling.hpp>
#include <kickstart/main_library/core/ns▸text_conversion/to-text/string-output-operator.hpp>

#include <memory>       // std::addressof

namespace kickstart::collection_support::_definitions {
    using namespace kickstart::failure_handling;    // hopefully, KS_FAIL_
    using namespace kickstart::language;            // Size, Index, Unsigned_size, Uindex
    using namespace kickstart::text_conversion;     // ""s, operator<<

    namespace kc = kickstart::collection_support;
    using kc::begin_ptr_of, kc::end_ptr_of;
    using kickstart::language::Size;

    using   std::out_of_range,
            std::addressof;

    // Wrt. `const` correctness class `Array_span_` is designed to act like a pointer.
    template< class Tp_item >
    class Array_span_
    {
    public:
        using Item = Tp_item;

    private:
        Item*       m_p_first;
        Item*       m_p_beyond;

        template< class Self >
        static auto at( const Index i, Self& self )
            -> auto&
        {
            hopefully( Uindex( i ) < Unsigned_size( self.size() ) )
                or KS_FAIL_( out_of_range, ""s << "Index value " << i << " is out of range." );
            return self[i];
        }

    public:
        Array_span_( const Type_<Item*> p_first, const Type_<Item*> p_beyond ):
            m_p_first( p_first ),
            m_p_beyond( p_beyond )
        {}

        Array_span_( const Type_<Item*> p_first, const Size n ):
            Array_span_( p_first, p_first + n )
        {}

        template< class Array >
        Array_span_( Array&& a ):
            m_p_first( begin_ptr_of( a ) ),
            m_p_beyond( end_ptr_of( a ) )
        {}

        auto data() const   -> Item*                    { return m_p_first; }
        auto size() const   -> Size                     { return m_p_beyond - m_p_first; }

        auto begin() const  -> Item*                    { return m_p_first; }
        auto end() const    -> Item*                    { return m_p_beyond; }

        auto cbegin() const -> const Item*              { return begin(); }
        auto cend() const   -> const Item*              { return end(); }

        auto item( const Index i ) const -> Item&       { return m_p_first[i]; }
        auto at( const Index i ) const -> Item&         { return at( i, *this ); }
        auto operator[]( const Index i ) const -> Item& { return item( i ); }
    };

    template< class Item >
    inline auto array_span_of( const Type_<Item*> p_first, const Type_<Item*> p_beyond )
        -> Array_span_<Item>
    { return Array_span_<Item>( p_first, p_beyond ); }

    template< class Item >
    inline auto array_span_of(  const Type_<Item*> p_first, const Size n )
        -> Array_span_<Item>
    { return Array_span_<Item>( p_first, n ); }

    template< class Array >
    inline auto array_span_of( Array& a )
        -> auto
    {
        using Item = decltype( *begin_ptr_of( a ) );
        return Array_span_<Item>( a );
    }

    template< class Type >      // TODO: enable_if or static_assert POD
    inline auto byte_span_of( const Type& o )
        -> Array_span_<const Byte>
    {
        const Type_<const void*> p_start = addressof( o );
        return Array_span_<const Byte>( reinterpret_cast<const Byte*>( p_start ), sizeof( o ) );
    }

    template< class Type >      // TODO: enable_if or static_assert POD
    inline auto byte_span_of( const Type_<const Type*> p, const Size n )
        -> Array_span_<const Byte>
    {
        return Array_span_<const Byte>( reinterpret_cast<const Byte*>( p ), n );
    }

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Array_span_,
        d::array_span_of,
        d::byte_span_of;
    }  // namespace exported names
}  // namespace kickstart::collection_support::_definitions

namespace kickstart::collection_support    { using namespace _definitions::exported_names; }
