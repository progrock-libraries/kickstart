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

#include <kickstart/root/core/language/Truth.hpp>

#include <type_traits>      // is_floating_point_v

// Important to not introduce possible future name conflicts with <math.h>, hence
// the “lx” (short for “language extension”) namespace.
namespace kickstart::language::lx::_definitions {
    using std::is_floating_point_v;

    namespace impl
    {
        // Essentially this is Horner's rule adapted to calculating a power, so that the
        // number of floating point multiplications is at worst O(log₂n).
        template< class Number_type >
        constexpr inline auto intpow_( const Number_type base, const int exponent )
            -> Number_type
        {
            Number_type result = 1;
            Number_type weight = base;
            for( int n = exponent; n != 0; weight *= weight ) {
                if( n % 2 != 0 ) {
                    result *= weight;
                }
                n /= 2;
            }
            return result;
        }
    }  // namespace impl

    template< class Number_type >
    constexpr inline auto intpow( const Number_type base, const int exponent )
        -> Number_type
    {
        // TODO: proper failure handling
        if( exponent < 0 and not is_floating_point_v<Number_type> ) {
            throw __func__;
        }
        return (0?0
            : exponent > 0?     impl::intpow_<Number_type>( base, exponent )
            : exponent == 0?    1
            :                   Number_type( 1.0/impl::intpow_<Number_type>( base, -exponent ) )
            );
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::intpow;
    }  // namespace exported names
}  // namespace kickstart::language::lx::_definitions

namespace kickstart::language::lx  { using namespace _definitions::exported_names; }
