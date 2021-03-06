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

#include <kickstart/core/language/lx/bits_per_.hpp>             // lx::bits_per_
#include <kickstart/core/language/Truth.hpp>                    // Truth
#include <kickstart/core/language/type-aliases.hpp>             // Type_

#include <assert.h>         // assert
#include <stdlib.h>         // abs, div

#include <type_traits>      // make_signed_t
#include <utility>


// Important to not introduce possible future name conflicts with <math.h>.
namespace kickstart::math::_definitions {
    namespace kl = kickstart::language;

    using   kl::lx::bits_per_, kl::Truth, kl::Type_;
    using   std::is_integral_v, std::is_unsigned_v, std::make_signed_t,
            std::enable_if_t;

    template< class Int,
        class = enable_if_t<is_integral_v<Int>>
        >
    inline auto compare( const Int a, const Int b )
        -> int
    { return (a < b? -1 : a == b? 0 : +1); }

    template< class Int >
    inline constexpr auto is_even( const Int x ) -> Truth { return x % 2 == 0; }

    template< class Int >
    inline constexpr auto is_odd( const Int x ) -> Truth { return x % 2 == 1; }

    template< class Int >
    inline constexpr auto sign_of( const Int x ) -> int { return (x > 0) - (x < 0); }

    template< class Int >
    inline auto msb_is_set_in( const Int value )
        -> Truth
    { return make_signed_t<Int>( value ) < 0; }

    template< class Int >
    inline auto lsb_is_set_in( const Int value )
        -> Truth
    { return !!(value & 1); }

    // Little endian result. Units must be/behave as unsigned integers.
    template< class Half_unit, class Unit >
    auto multiply_by_parts( const Unit a, const Unit b, const Type_<Unit*> p_result_items  )
    {
        static_assert( is_unsigned_v<Unit> );
        assert( p_result_items );

        if constexpr( bits_per_<Unit> <= 32 ) {
            const int unit_radix = 1ULL << bits_per_<Unit>;
            p_result_items[0] = Unit( 1ULL*a*b % unit_radix );
            p_result_items[1] = Unit( 1ULL*a*b / unit_radix );
        } else {
            static_assert( is_unsigned_v<Half_unit> );
            static_assert( sizeof( Unit ) == 2*sizeof( Half_unit ) );

            const int half_shift = bits_per_<Half_unit>;
            constexpr auto half_mask = Half_unit( -1 );

            const Half_unit parts_a[2] = { Half_unit( a & half_mask ), Half_unit( a >> half_shift ) };
            const Half_unit parts_b[2] = { Half_unit( b & half_mask ), Half_unit( b >> half_shift ) };

            constexpr auto one = Unit( 1 );
            const Unit  low         = one * parts_a[0] * parts_b[0];
            const Unit  mid_1       = one * parts_a[0] * parts_b[1];
            const Unit  mid_2       = one * parts_a[1] * parts_b[0];
            const Unit  mid         = mid_1 + mid_2;
            const Truth mid_carry   = (mid < mid_1);
            const Unit  high        = one * parts_a[1] * parts_b[1];

            p_result_items[0] = low + ((mid & half_mask) << half_shift);
            const Truth part_0_carry = (p_result_items[0] < low);
            p_result_items[1] = Unit()
                + Unit( +part_0_carry )
                + (mid >> half_shift) + (Unit( +mid_carry ) << half_shift)
                + high;
        }
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::compare,
        d::is_even, d::is_odd,
        d::sign_of,
        d::msb_is_set_in, d::lsb_is_set_in,
        d::multiply_by_parts;
    }  // namespace exported names
}  // namespace kickstart::math::_definitions

namespace kickstart::math   { using namespace _definitions::exported_names; }
