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

#include <kickstart/core/language/Truth.hpp>

#include <stddef.h>         // size_t

#include <numeric>          // accumulate


// Important to not introduce possible future name conflicts with <math.h>.
namespace kickstart::math::_definitions {
    using   std::accumulate;

    template< class C >
    struct Collection_traits_
    {
        using Item = typename C::value_type;
    };

    template< class Array_item, size_t n >
    struct Collection_traits_<Array_item[n]>
    {
        using Item = Array_item;
    };

    template< class C >
    using Item_type_of_ = typename Collection_traits_<C>::Item;

    template< class Collection, class Number = Item_type_of_<Collection> >
    inline auto sum_of( const Collection& numbers )
        -> Number
    {
        Number result = 0;
        for( const Number x : numbers ) { result += x; }
        return result;
    }

    namespace d = _definitions;
    namespace exports{ using
        d::sum_of;
    }  // namespace exports
}  // namespace kickstart::math::_definitions

namespace kickstart::math   { using namespace _definitions::exports; }
