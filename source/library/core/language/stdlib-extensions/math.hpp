// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../../assertion-headers/assert-reasonable-compiler.hpp"

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

#include <cmath>        // Special functions, for completeness.
#include <math.h>       // DBL_MANT_DIG

// Important to not introduce possible future name conflicts with <math.h>.
namespace kickstart::math::_definitions {

    template< class Int = int >
    inline constexpr auto is_even( const Int x ) -> bool { return x % 2 == 0; }

    template< class Int = int >
    inline constexpr auto is_odd( const Int x ) -> bool { return x % 2 == 1; }

    constexpr double e                  = 2.71828182845904523536;
    constexpr double log2e              = 1.44269504088896340736;
    constexpr double log10e             = 0.434294481903251827651;
    constexpr double ln2                = 0.693147180559945309417;
    constexpr double ln10               = 2.30258509299404568402;
    constexpr double pi                 = 3.14159265358979323846;
    constexpr double inv_sqrt_pi        = 1.12837916709551257390;
    constexpr double sqrt2              = 1.41421356237309504880;
    constexpr double inv_sqrt2          = 0.707106781186547524401;

    namespace impl
    {

        template< class Fp_type >
        constexpr inline auto intpow_( const Fp_type base, const int exponent )
            -> Fp_type
        {
            Fp_type result = 1;
            Fp_type weight = base;
            for( int n = exponent; n != 0; weight *= weight ) {
                if( is_odd( n ) ) {
                    result *= weight;
                }
                n /= 2;
            }
            return result;
        }
    }  // namespace impl

    // Essentially this is Horner's rule adapted to calculating a power, so that the
    // number of floating point multiplications is at worst O(log2(n)).
    template< class Fp_type = double>
    constexpr inline auto intpow_( const Fp_type base, const int exponent )
        -> Fp_type
    {
        return (0?0
            : exponent == 0?        1.0
            : exponent < 0?         1.0/impl::intpow_<Fp_type>( base, -exponent )
            :                       impl::intpow_<Fp_type>( base, exponent )
            );
    }

    constexpr inline auto intpow( const double base, const int exponent )
        -> double
    { return intpow_<double>( base, exponent ); }
        
    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::is_even, d::is_odd,
        d::e, d::log2e, d::log10e, d::ln2, d::ln10, d::pi, d::inv_sqrt_pi, d::sqrt2, d::inv_sqrt2,
        d::intpow_, d::intpow;
    }  // namespace exported names
}  // namespace kickstart::math::_definitions

namespace kickstart::math           { using namespace _definitions::exported_names; }
namespace kickstart::core           { namespace math = kickstart::math; }
