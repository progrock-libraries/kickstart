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

#include <kickstart/main_library/core/ns▸language/operations/intpow.hpp>        // lx::intpow_

#include <kickstart/c/float.hpp>    // DBL_MANT_DIG

namespace kickstart::basic_type_properties::_definitions {
    namespace lx = kickstart::language::lx;

    template< class Fp_type > constexpr int bits_per_mantissa_of_ = 0;  // "= 0" for clang.
    template<> constexpr int bits_per_mantissa_of_<float>           = FLT_MANT_DIG;
    template<> constexpr int bits_per_mantissa_of_<double>          = DBL_MANT_DIG;
    template<> constexpr int bits_per_mantissa_of_<long double>     = LDBL_MANT_DIG;

    template< class Fp_type >
    constexpr Fp_type largest_exact_integer_of_ =
        lx::intpow( Fp_type( 2 ), bits_per_mantissa_of_<Fp_type> - 1 ) +
        (lx::intpow( Fp_type( 2 ), bits_per_mantissa_of_<Fp_type> - 1 ) - 1);


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::bits_per_mantissa_of_,
        d::largest_exact_integer_of_;
    }  // namespace exported names
}  // namespace kickstart::basic_type_properties::_definitions

namespace kickstart::basic_type_properties  { using namespace _definitions::exported_names; }
