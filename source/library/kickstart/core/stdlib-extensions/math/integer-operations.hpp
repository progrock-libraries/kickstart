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

#include <stdlib.h>         // abs, div

#include <type_traits>      // make_signed_t


// Important to not introduce possible future name conflicts with <math.h>.
namespace kickstart::math::_definitions {
    using kickstart::language::Truth;
    using   std::make_signed_t;

    template< class Int >
    inline constexpr auto is_even( const Int x ) -> Truth { return x % 2 == 0; }

    template< class Int >
    inline constexpr auto is_odd( const Int x ) -> Truth { return x % 2 == 1; }

    template< class Int >
    inline constexpr auto signum_of( const Int x ) -> int { return (x > 0) - (x < 0); }

    template< class Int >
    inline auto msb_is_set_in( const Int value )
        -> Truth
    { return make_signed_t<Int>( value ) < 0; }

    template< class Int >
    inline auto lsb_is_set_in( const Int value )
        -> Truth
    { return !!(value & 1); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::is_even, d::is_odd,
        d::signum_of,
        d::msb_is_set_in, d::lsb_is_set_in;
    }  // namespace exported names
}  // namespace kickstart::math::_definitions

namespace kickstart::math           { using namespace _definitions::exported_names; }
