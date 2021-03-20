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

#include <kickstart/core/language/lx/bits_per_.hpp>     // lx::bits_per_

#include <stddef.h>     // size_t, ptrdiff_t
#include <stdint.h>     // int32_t, int64_t

namespace kickstart::language::_definitions {
    using lx::bits_per_;

    template< class T > using Type_ = T;

    using Size                  = ptrdiff_t;
    using Index                 = ptrdiff_t;
    using Unsigned_size         = size_t;
    using Unsigned_index        = size_t;
    using Byte                  = unsigned char;

    using Int32                 = int32_t;
    using Unsigned_int32        = uint32_t;
    using Number32              = float;
    using Int64                 = int64_t;
    using Unsigned_int64        = uint64_t;
    using Number64              = double;

    static_assert( bits_per_<Number32> >= 32 );
    static_assert( bits_per_<Number64> >= 64 );

    using Integer   = Int64;
    using Number    = Number64;

    using C_str     = const char*;
    using C_wstr    = const wchar_t*;

    template< int n_bits >      struct Int_t_;
    template< int n_bits >      using Int_ = typename Int_t_<n_bits>::T;

    template< int n_bits >      struct Uint_t_;
    template< int n_bits >      using Uint_ = typename Uint_t_<n_bits>::T;

    template<> struct Int_t_< 8> { using T =  int8_t; };
    template<> struct Int_t_<16> { using T = int16_t; };
    template<> struct Int_t_<32> { using T = int32_t; };
    template<> struct Int_t_<64> { using T = int64_t; };

    template<> struct Uint_t_< 8> { using T =  uint8_t; };
    template<> struct Uint_t_<16> { using T = uint16_t; };
    template<> struct Uint_t_<32> { using T = uint32_t; };
    template<> struct Uint_t_<64> { using T = uint64_t; };


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Type_,
        d::Size, d::Index, d::Unsigned_size, d::Unsigned_index, d::Byte,
        d::Int32, d::Unsigned_int32, d::Number32,
        d::Int64, d::Unsigned_int64, d::Number64,
        d::Integer, d::Number,
        d::C_str, d::C_wstr,
        d::Int_, d::Uint_;
    }  // namespace exported names
}  // namespace kickstart::language::_definitions

namespace kickstart::language       { using namespace _definitions::exported_names; }
