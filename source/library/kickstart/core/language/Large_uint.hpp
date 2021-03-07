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

#include <kickstart/core/language/Tag_.hpp>
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/core/stdlib-extensions/limits.hpp>      // bits_per_
#include <kickstart/core/stdlib-extensions/strings.hpp>     // spaces
#include <kickstart/core/stdlib-extensions/math/integer-operations.hpp>

#include <stdint.h>         // Unit

#include <array>
#include <bitset>
#include <string>
#include <utility>          // swap

namespace kickstart::tag {
    using kickstart::language::Tag_;
    using From_parts        = Tag_<struct Struct_from_parts>;
    using Uninitialized     = Tag_<struct Struct_uninitialized>;
}  // namespace kickstart::tag

namespace kickstart::language::_definitions {
    using namespace kickstart::math;        // xxx_is_set_in, multiply_by_parts
    using namespace kickstart::strings;

    using   kickstart::limits::bits_per_;
    using   std::array,
            std::bitset,
            std::string,
            std::swap;

    class Large_uint
    {
    public:
        using Half_unit = uint32_t;
        using Unit      = uint64_t;
        using Parts     = array<Unit, 2>;       // 0 => lsp, 1 => msp.
        static constexpr int n_bits = 2*bits_per_<Unit>;

    private:
        using Self = Large_uint;

        Parts   m_parts;

    public:
        Large_uint(): m_parts() {}
        Large_uint( tag::Uninitialized ) {}
        Large_uint( const Unit value ): m_parts{ value, 0 } {}
        Large_uint( tag::From_parts, const Unit lsp, const Unit msp ): m_parts{ lsp, msp } {}

        Large_uint( const Self& ) = default;
        auto operator=( const Self& ) -> Self& = default;

        inline auto parts() const -> const Parts&;
        inline auto to_bitset() const -> bitset<n_bits>;
        inline auto modulo_unit() const -> Unit;
        inline auto is_in_unit_range() const -> Truth;
        inline auto operator~() const -> Self;

        inline void operator++();
        inline void operator--();

        inline void operator+=( const Self& other );
        inline void operator-=( const Self& other );

        inline void shift_left();
        inline void shift_right();

        struct Divmod_result;
        inline auto divmod_unit( const Unit b ) const -> Divmod_result;
    };

    inline auto operator+( const Large_uint& ) -> Large_uint;
    inline auto operator-( const Large_uint& ) -> Large_uint;

    inline auto operator+( const Large_uint& a, const Large_uint& b ) -> Large_uint;
    inline auto operator-( const Large_uint& a, const Large_uint& b ) -> Large_uint;
    inline auto operator*( const Large_uint::Unit a, const Large_uint& b ) -> Large_uint;
    inline auto operator/( const Large_uint& a, const Large_uint::Unit b ) -> Large_uint;
    inline auto operator%( const Large_uint& a, const Large_uint::Unit b ) -> Large_uint;

    inline auto compare( const Large_uint& a, const Large_uint& b ) -> int;
    inline auto operator<( const Large_uint& a, const Large_uint& b ) -> Truth;
    inline auto operator<=( const Large_uint& a, const Large_uint& b ) -> Truth;
    inline auto operator==( const Large_uint& a, const Large_uint& b ) -> Truth;
    inline auto operator>=( const Large_uint& a, const Large_uint& b ) -> Truth;
    inline auto operator>( const Large_uint& a, const Large_uint& b ) -> Truth;
    inline auto operator!=( const Large_uint& a, const Large_uint& b ) -> Truth;

    inline auto str( const Large_uint& v ) -> string;


    //--------------------------------------------------------------------------------------------------

    inline auto Large_uint::parts() const
        -> const Parts&
    { return m_parts; }

    inline auto Large_uint::to_bitset() const
        -> bitset<n_bits>
    {
        auto bits = bitset<n_bits>( m_parts[0] );
        for( int i = 0; i < bits_per_<Unit>; ++i ) {
            const int bit = (m_parts[1] >> i) & 1;
            bits.set( i + bits_per_<Unit>, !!bit );
        }
        return bits;
    }

    inline auto Large_uint::modulo_unit() const
        -> Unit
    { return m_parts[0]; }

    inline auto Large_uint::is_in_unit_range() const
        -> Truth
    { return m_parts[1] == 0; }

    inline auto Large_uint::operator~() const
        -> Self
    {
        auto result = Self( tag::Uninitialized() );
        for( int i = 0; i < 2; ++i ) {
            result.m_parts[i] = ~m_parts[i];
        }
        return result;
    }

    inline void Large_uint::operator++()
    {
        ++m_parts[0];
        if( m_parts[0] == 0 ) {
            ++m_parts[1];
        }
    }

    inline void Large_uint::operator--()
    {
        if( m_parts[0] == 0 ) {
            --m_parts[1];
        }
        --m_parts[0];
    }

    inline void Large_uint::operator+=( const Self& other )
    {
        m_parts[0] += other.m_parts[0];
        const Truth carry = (m_parts[0] < other.m_parts[0]);
        m_parts[1] += Unit( other.m_parts[1] + +carry );
    }

    inline void Large_uint::operator-=( const Self& other )
    {
        *this += -other;
    }

    inline void Large_uint::shift_left()
    {
        const Truth carry = msb_is_set_in( m_parts[0] );
        m_parts[0] <<= 1;
        m_parts[1] <<= 1;
        m_parts[1] |= +carry;
    }

    inline void Large_uint::shift_right()
    {
        const Truth carry = lsb_is_set_in( m_parts[1] );
        m_parts[1] >>= 1;
        m_parts[0] >>= 1;
        m_parts[0] |= (Unit( +carry ) << (bits_per_<Unit> - 1));
    }

    struct Large_uint::Divmod_result
    {
        Large_uint   remainder;
        Large_uint   quotient;
    };

    inline auto Large_uint::divmod_unit( const Large_uint::Unit b ) const
        -> Divmod_result
    {
        #ifndef KS_TEST_DIVISION_PLEASE
            if( m_parts[1] == 0 ) {
                return {Unit( m_parts[0]%b ), Unit( m_parts[0]/b )};
            }
        #endif

        Unit divisor = b;
        int n_shifts = 0;
        while( not msb_is_set_in( divisor ) ) {
            divisor <<= 1;
            ++n_shifts;
        }

        const int n_q_digits = 1 + n_shifts + bits_per_<Unit>;
        Divmod_result result = {*this, 0};
        Truth carry = false;
        for( int i = 0;; ) {
            if( carry or divisor <= result.remainder.m_parts[1] ) {
                result.remainder.m_parts[1] -= divisor;
                result.quotient.m_parts[0] |= 1;
            }

            ++i;
            if( i == n_q_digits ) {
                break;
            }

            result.quotient.shift_left();
            carry = msb_is_set_in( result.remainder.parts()[1] );
            result.remainder.shift_left();
        }
        assert( result.remainder.m_parts[0] == 0 );
        swap( result.remainder.m_parts[0], result.remainder.m_parts[1] );
        result.remainder.m_parts[0] >>= n_shifts;
        return result;
    }

    inline auto operator+( const Large_uint& value )
        -> Large_uint
    { return value; }

    inline auto operator-( const Large_uint& value )
        -> Large_uint
    {
        Large_uint result = ~value;
        ++result;
        return result;
    }

    inline auto operator+( const Large_uint& a, const Large_uint& b )
        -> Large_uint
    {
        Large_uint result = a;
        result += b;
        return result;
    }

    inline auto operator-( const Large_uint& a, const Large_uint& b )
        -> Large_uint
    { return a + -b; }

    inline auto operator*( const Large_uint::Unit a, const Large_uint& b )
        -> Large_uint
    {
        Large_uint::Unit parts_product[2];
        multiply_by_parts<Large_uint::Half_unit>( a, b.parts()[0], parts_product );
        return Large_uint( tag::From_parts(),
            parts_product[0],
            parts_product[1] + a*b.parts()[1]
        );
    }

    inline auto operator/( const Large_uint& a, const Large_uint::Unit b )
        -> Large_uint
    { return a.divmod_unit( b ).quotient; }

    inline auto operator%( const Large_uint& a, const Large_uint::Unit b )
        -> Large_uint
    { return a.divmod_unit( b ).remainder; }

    inline auto compare( const Large_uint& a, const Large_uint& b )
        -> int
    {
        using Unit = Large_uint::Unit;
        static const auto compare_units = []( const Unit a, const Unit b )
            -> int
        { return (a < b? -1 : a == b? 0 : +1); };

        if( const int r = compare_units( a.parts()[1], b.parts()[1] ) ) {
            return r;
        }
        return compare_units( a.parts()[0], b.parts()[0] );
    }

    inline auto operator<( const Large_uint& a, const Large_uint& b )
        -> Truth
    { return (compare( a, b ) < 0); }

    inline auto operator<=( const Large_uint& a, const Large_uint& b )
        -> Truth
    { return (compare( a, b ) <= 0); }

    inline auto operator==( const Large_uint& a, const Large_uint& b )
        -> Truth
    { return (compare( a, b ) == 0); }

    inline auto operator>=( const Large_uint& a, const Large_uint& b )
        -> Truth
    { return (compare( a, b ) >= 0); }

    inline auto operator>( const Large_uint& a, const Large_uint& b )
        -> Truth
    { return (compare( a, b ) > 0); }

    inline auto operator!=( const Large_uint& a, const Large_uint& b )
        -> Truth
    { return (compare( a, b ) != 0); }

    inline auto str( const Large_uint& v )
        -> string
    {
        if( v == 0 ) { return "0"; }

        string digits;
        Large_uint a = v;
        while( a != 0 ) {
            const auto r = a.divmod_unit( 10 );
            digits += "0123456789"[r.remainder.modulo_unit()];
            a = r.quotient;
        }
        reverse( begin( digits ), end( digits ) );
        return digits;
    }

}  // namespace kickstart::language::_definitions
