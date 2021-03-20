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
#include <kickstart/core/language/type-aliases.hpp>             // Type_, Uint_
#include <kickstart/core/stdlib-extensions/math/general-number-operations.h>    // compare

namespace kickstart::large_integers::_definitions {
    namespace kl = kickstart::language;

    using   kl::lx::bits_per_,
            kl::Truth,
            kl::Type_,
            kl::Uint_;
    using   kickstart::math::compare;
    using   std::is_unsigned_v;

    template< class Uint_param >
    struct Uint_double_of_
    {
        static_assert( is_unsigned_v<Uint_param> );
        using Part = Uint_param;

        Part parts[2];          // Parts in little endian order.

        friend
        constexpr auto compare( const Uint_double_of_& a, const Uint_double_of_& b )
            -> int
        {
            if( const int r = compare( a.parts[1], b.parts[1] ) ) {
                return r;
            }
            return compare( a.parts[0], b.parts[0] );
        }

        constexpr static auto product_of( const Part a, const Part b )
            -> Uint_double_of_
        {
            if constexpr( bits_per_<Part> <= 32 ) {
                const int unit_radix = 1ULL << bits_per_<Part>;
                return { Part( 1ULL*a*b % unit_radix ), Part( 1ULL*a*b / unit_radix ) };
            } else {
                using Half_unit = Uint_<bits_per_<Part>/2>;

                const int half_shift = bits_per_<Half_unit>;
                constexpr auto half_mask = Half_unit( -1 );

                const Half_unit parts_a[2] = { Half_unit( a & half_mask ), Half_unit( a >> half_shift ) };
                const Half_unit parts_b[2] = { Half_unit( b & half_mask ), Half_unit( b >> half_shift ) };

                constexpr auto one = Part( 1 );
                const Part  low         = one * parts_a[0] * parts_b[0];
                const Part  mid_1       = one * parts_a[0] * parts_b[1];
                const Part  mid_2       = one * parts_a[1] * parts_b[0];
                const Part  mid         = mid_1 + mid_2;
                const Truth mid_carry   = (mid < mid_1);
                const Part  high        = one * parts_a[1] * parts_b[1];

                Uint_double_of_<Part> result = {};  // Initialized to silence g++ compiler.
                result.parts[0] = low + ((mid & half_mask) << half_shift);
                const Truth part_0_carry = (result.parts[0] < low);
                result.parts[1] = Part()
                    + Part( +part_0_carry )
                    + (mid >> half_shift) + (Part( +mid_carry ) << half_shift)
                    + high;
                return result;
            }
        }
    };


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Uint_double_of_;
    }  // namespace exported names
}  // namespace kickstart::large_integers::_definitions

namespace kickstart::large_integers   { using namespace _definitions::exported_names; }
