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

#include <kickstart/core/language/stdlib-includes/basics.hpp>    // size

#include <iterator>

namespace kickstart::collection_util::_definitions {
    using   std::next;

    template< class C >
    inline auto begin_of( C& c )
        -> auto
    { using std::begin; return begin( c ); }

    template< class C >
    inline auto end_of( C& c )
        -> auto
    { using std::end; return end( c ); }

    template< class Iterator >
    class Iteration_
    {
        Iterator    m_it_begin;
        Iterator    m_it_end;

    public:
        Iteration_( const Iterator it_begin, const Iterator it_end ):
            m_it_begin( it_begin ),
            m_it_end( it_end )
        {}

        template< class Collection >
        Iteration_( Collection& c ):
            m_it_begin( begin_of( c ) ),
            m_it_end( end_of( c ) )
        {}

        auto begin() const -> Iterator { return m_it_begin; }
        auto end() const -> Iterator { return m_it_end; }
    };

    template< class Iterator >
    inline auto all_of( const Iterator it_begin, const Iterator it_end )
        -> Iteration_<Iterator>
    { return {it_begin, it_end}; }

    template< class Collection >
    inline auto all_of( Collection&& c )
        -> Iteration_<decltype( begin_of( c ) )>
    { return {c}; }

    template< class Iterator >
    inline auto head_of( const Iterator it_begin, const Iterator it_end )
        -> Iteration_<Iterator>
    { return {it_begin, next( it_begin )}; }

    template< class Collection >
    inline auto head_of( Collection&& c )
        -> Iteration_<decltype( begin_of( c ) )>
    { return {begin_of( c ), next( begin_of( c ) )}; }

    template< class Iterator >
    inline auto tail_of( const Iterator it_begin, const Iterator it_end )
        -> Iteration_<Iterator>
    { return {next( it_begin ), it_end}; }

    template< class Collection >
    inline auto tail_of( Collection&& c )
        -> Iteration_<decltype( begin_of( c ) )>
    { return {next( begin_of( c ) ), end_of( c )}; }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::begin_of, d::end_of,
        d::Iteration_,
        d::all_of, d::head_of, d::tail_of;
    }  // namespace exported names
}  // namespace kickstart::core::collection_util::_definitions

namespace kickstart::collection_util    { using namespace _definitions::exported_names; }
