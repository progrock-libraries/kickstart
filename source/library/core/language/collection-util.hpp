// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../assertion-headers/$-assert-reasonable-compiler.hpp"

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

#include "../failure-handling.hpp"
#include "../text_conversion/to-text/string-output-operator.hpp"
#include "stdlib-includes/basics.hpp"
#include "type_aliases.hpp"

namespace kickstart::language::_definitions {
    using namespace kickstart::failure_handling;    // hopefully, KS_FAIL_
    using namespace kickstart::language;            // Size, Index, Unsigned_size, Unsigned_index
    using namespace kickstart::text_conversion;     // ""s, operator<<

    using kickstart::core::ssize;
    using std::out_of_range;

    template< class Result, class Collection >
    inline auto ssize_( const Collection& c )
        -> Result
    { return static_cast<Result>( ssize( c ) ); }

    template< class Collection >
    inline auto int_size( const Collection& c )
        -> int
    { return ssize_<int>( c ); }

    template< class Array >        // TODO: enable_if random access
    inline auto begin_ptr_of( Array& a )
        -> auto
    { return a.data(); }

    template< class Item, Size n >
    inline auto begin_ptr_of( Item (&a)[n] )
        -> auto
    { return &a[0]; }

    template< class Array >        // TODO: enable_if random access
    inline auto end_ptr_of( Array& a )
        -> auto
    { return a.data() + a.size(); }

    template< class Item, Size n >
    inline auto end_ptr_of( Item (&a)[n] )
        -> auto
    { return &a[0] + n; }

    // Wrt. `const` correctness class `Array_span_` is designed to act like an array.
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
            hopefully( Unsigned_index( i ) < Unsigned_size( self.size() ) )
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
        Array_span_( Array& a ):
            m_p_first( begin_ptr_of( a ) ),
            m_p_beyond( end_ptr_of( a ) )
        {}

        auto data() -> Item* { return m_p_first; }
        auto data() const -> const Item* { return m_p_first; }
        auto size() const -> Size { return m_p_beyond - m_p_first; }

        auto begin() -> Item* { return m_p_first; }
        auto begin() const -> const Item* { return m_p_first; }
        auto cbegin() const -> const Item* { return m_p_first; }
        auto end() -> Item* { return m_p_beyond; }
        auto end() const -> const Item* { return m_p_beyond; }
        auto cend() const -> const Item* { return m_p_beyond; }

        auto operator[]( const Index i ) -> Item& { return m_p_first[i]; }
        auto operator[]( const Index i ) const -> const Item& { return m_p_first[i]; }

        auto at( const Index i ) -> Item& { return at( i, *this ); }
        auto at( const Index i ) const -> const Item& { return at( i, *this ); }
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

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::ssize_, d::int_size,
        d::begin_ptr_of, d::end_ptr_of,
        d::Array_span_, d::array_span_of;
    }  // namespace exported names
}  // namespace kickstart::language::_definitions

namespace kickstart::language       { using namespace _definitions::exported_names; }
namespace kickstart::core           { using namespace language; }
