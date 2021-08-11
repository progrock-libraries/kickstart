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

#include <kickstart/root/core/namespace▸language/operations/bits_per_.hpp>                            // lx::bits_per_
#include <kickstart/root/core/namespace▸language/types/Truth.hpp>                                     // Truth
#include <kickstart/root/core/namespace▸language/types/type-aliases.hpp>                              // Type_, Uint_
#include <kickstart/root/core/stdlib-extensions/calc/integer-operations.hpp>                // lsb_is_set_in, msb_is_set_in
#include <kickstart/root/core/stdlib-extensions/calc/templated-numerical-operations.hpp>    // compare

#include <kickstart/c/assert.hpp>       // assert

#include <type_traits>      // is_unsigned_v
#include <utility>          // swap

namespace kickstart::large_integers::_definitions {
    namespace kl = kickstart::language;
    namespace km = kickstart::math;

    using   kl::lx::bits_per_,
            kl::Truth,
            kl::Type_,
            kl::Uint_;
    using   km::lsb_is_set_in, km::msb_is_set_in, km::compare;
    using   std::is_unsigned_v,
            std::swap;

    template< class Uint_param >
    struct Uint_double_of_
    {
        static_assert( is_unsigned_v<Uint_param> );
        using Part = Uint_param;

        Part parts[2];          // Parts in little endian order.

        inline constexpr auto operator~() const -> Uint_double_of_;
        inline constexpr void shift_left();
        inline constexpr void shift_right();

        inline constexpr static auto product_of( const Part a, const Part b )
            -> Uint_double_of_;

        struct Divmod_result;
        inline constexpr static auto quotient_of( const Uint_double_of_& a, const Part b )
            -> Divmod_result;
    };

    template< class Uint_param >
    inline constexpr auto compare(
        const Uint_double_of_<Uint_param>&   a,
        const Uint_double_of_<Uint_param>&   b
        ) -> int
    {
        if( const int r = compare( a.parts[1], b.parts[1] ) ) {
            return r;
        }
        return compare( a.parts[0], b.parts[0] );
    }

    template< class Uint_param >
    inline constexpr auto Uint_double_of_<Uint_param >::operator~() const
        -> Uint_double_of_
    { return {~parts[0], ~parts[1]}; }

    template< class Uint_param >
    inline constexpr void Uint_double_of_<Uint_param >::shift_left()
    {
        const Truth carry = msb_is_set_in( parts[0] );
        parts[0] <<= 1;  parts[1] <<= 1;
        parts[1] |= +carry;
    }

    template< class Uint_param >
    inline constexpr void Uint_double_of_<Uint_param >::shift_right()
    {
        const Truth carry = lsb_is_set_in( parts[1] );
        parts[1] >>= 1;  parts[0] >>= 1;
        parts[0] |= (Part( +carry ) << (bits_per_<Part> - 1));
    }

    template< class Uint_param >
    constexpr auto Uint_double_of_<Uint_param >::product_of(
        const Part      a,
        const Part      b
        ) -> Uint_double_of_
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

    template< class Uint_param >
    struct Uint_double_of_<Uint_param >::Divmod_result
    {
        Uint_double_of_     remainder;
        Uint_double_of_     quotient;
    };

    template< class Uint_param >
    inline constexpr auto Uint_double_of_<Uint_param>::quotient_of(
        const Uint_double_of_&      a,
        const Part                  b
        ) -> Divmod_result
    {
        if( b == 0 ) { return Divmod_result{ ~Uint_double_of_(), ~Uint_double_of_() }; }

        #ifndef KS_TEST_DIVISION_PLEASE
            if( a.parts[1] == 0 ) {
                return Divmod_result{ { Part( a.parts[0] % b ) }, { Part( a.parts[0] / b ) } };
            }
        #endif

        Part divisor = b;
        int n_shifts = 0;
        while( not msb_is_set_in( divisor ) ) {
            divisor <<= 1;
            ++n_shifts;
        }

        const int n_quotient_digits = 1 + n_shifts + bits_per_<Part>;
        Divmod_result result = {a, 0};
        Truth carry = false;
        for( int i = 0;; ) {
            if( carry or divisor <= result.remainder.parts[1] ) {
                result.remainder.parts[1] -= divisor;
                result.quotient.parts[0] |= 1;
            }

            ++i;
            if( i == n_quotient_digits ) {
                break;
            }

            result.quotient.shift_left();
            carry = msb_is_set_in( result.remainder.parts[1] );
            result.remainder.shift_left();
        }
        assert( result.remainder.parts[0] == 0 );
        swap( result.remainder.parts[0], result.remainder.parts[1] );
        result.remainder.parts[0] >>= n_shifts;
        return result;
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Uint_double_of_,
        d::compare;
    }  // namespace exported names
}  // namespace kickstart::large_integers::_definitions

namespace kickstart::large_integers   { using namespace _definitions::exported_names; }
