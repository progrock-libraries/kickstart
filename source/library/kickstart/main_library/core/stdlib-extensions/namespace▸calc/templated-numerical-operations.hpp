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

#include <kickstart/main_library/core/namespace▸language/types/Truth.hpp>                    // Truth

#include <type_traits>


// Important to not introduce possible future name conflicts with <math.h>.
namespace kickstart::calc::_definitions {

    using   std::is_arithmetic_v,
            std::enable_if_t;

    template< class Number,
        class = enable_if_t<is_arithmetic_v<Number>>
        >
    inline constexpr auto compare( const Number a, const Number b )
        -> int
    { return (a < b? -1 : a == b? 0 : +1); }

    template< class Number,
        class = enable_if_t<is_arithmetic_v<Number>>
        >
    inline constexpr auto sign_of( const Number x )
        -> int
    { return (x > 0) - (x < 0); }

    template< class Number,
        class = enable_if_t<is_arithmetic_v<Number>>
        >
    inline constexpr auto abs( const Number v )
        -> Number
    { return (v < 0? -v : v); }

    template< class Number,
        class = enable_if_t<is_arithmetic_v<Number>>
        >
    inline constexpr auto squared( const Number x ) 
        -> Number
    { return x*x; }

    template< class Number,
        class = enable_if_t<is_arithmetic_v<Number>>
        >
    inline constexpr auto cubed( const Number x ) 
        -> Number
    { return x*x*x; }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::compare,
        d::sign_of,
        d::abs,
        d::squared,
        d::cubed;
    }  // namespace exported names
}  // namespace kickstart::calc::_definitions

namespace kickstart::calc   { using namespace _definitions::exported_names; }
