// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/core/language/assertion-headers/~assert-reasonable-compiler.hpp>

/// `Truth` is a drop-in replacement for `bool` without implicit conversion from/to types other
/// than `bool`.

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

#include <stdint.h>         // uint64_t

#include <array>

namespace kickstart::tag {
    using From_64bit = struct Struct_from_64bit*;
    constexpr auto from_64bit = From_64bit();

    using Uninitialized = struct Struct_uninitialized*;
    constexpr auto uninitialized = Uninitialized();
}  // namespace kickstart::tag

namespace kickstart::language::_definitions {
    using   std::array;

    class Limited_unsigned_int128
    {
        using Self = Limited_unsigned_int128;

        array<uint64_t, 2>  m_parts;        // 0 => lsp, 1 => msp.

    public:
        Limited_unsigned_int128():
            m_parts()
        {}

        Limited_unsigned_int128( tag::Uninitialized )
        {}

        Limited_unsigned_int128( tag::From_64bit, const uint64_t value ):
            m_parts{ value, 0 }
        {}

        auto operator~() const -> Self
        {
            Self result = *this;
            for( auto& part: result.m_parts ) { part = ~part; }
            return result;
        }

        void shift_left()
        {
            const Truth carry = (int64_t( m_parts[0] ) < 0);
            m_parts[0] <<= 1;
            m_parts[1] <<= 1;
            m_parts[1] |= +carry;
        }

        void shift_right()
        {
            const Truth carry = ((m_parts[1] & 1) != 0);
            m_parts[1] >>= 1;
            m_parts[0] >>= 1;
            m_parts[0] |= (uint64_t( +carry ) << 31);
        }

        auto operator++() -> Self&
        {
            ++m_parts[0];
            if( m_parts[0] == 0 ) {
                ++m_parts[1];
            }
            return *this;
        }

        auto operator--() -> Self&
        {
            if( m_parts[0] == 0 ) {
                --m_parts[1];
            }
            --m_parts[0];
            return *this;
        }

        friend
        auto operator+( const Self& a, const Self& b ) -> Self
        {
            auto result = Self( tag::uninitialized );
            result.m_parts[0] = a.m_parts[0] + b.m_parts[0];
            const Truth carry = (result.m_parts[0] < a.m_parts[0]);
            result.m_parts[1] = a.m_parts[1] + b.m_parts[1] + +carry;
            return result;
        }

        auto operator-() const -> Self
        {
            Self result = ~*this;
            ++result;
            return result;
        }

        friend
        auto operator-( const Self& a, const Self& b ) -> Self
        {
            return a + -b;
        }

        static
        auto mul64( const uint64_t a, const uint64_t b ) -> Self
        {
            const array<uint32_t, 2>    parts_a = { a & 0xFFFFFFFF, a >> 32 };
            const array<uint32_t, 2>    parts_b = { b & 0xFFFFFFFF, b >> 32 };

            constexpr auto one = uint64_t( 1 );
            const uint64_t  low         = one * parts_a[0] * parts_b[0];
            const uint64_t  mid_1       = one * parts_a[0] * parts_b[1];
            const uint64_t  mid_2       = one * parts_a[1] * parts_b[0];
            const uint64_t  mid         = mid_1 + mid_2;
            const Truth     mid_carry   = (mid < mid_1);
            const uint64_t  high        = one * parts_a[1] * parts_b[1];

            auto result = Self( tag::uninitialized );
            result.m_parts[0] = low + ((mid & 0xFFFFFFFF) << 32);
            const Truth part_0_carry = (result.m_parts[0] < low);
            result.m_parts[1] = (+part_0_carry) + (mid >> 32) + (uint64_t(+mid_carry) << 32) + high;
            return result;
        }

        friend
        auto operator*( const uint64_t a, const Self& b ) -> Self
        {
            Self result = mul64( b.m_parts[0], a );
            result.m_parts[1] += b.m_parts[1]*a;
            return result;
        }

        friend
        auto operator/( const Self& a, const uint64_t b ) -> Self
        {
            (void) a;  (void) b;
            return {};
        }
    };

}  // namespace kickstart::language::_definitions
