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

#if 1
#   include <kickstart/faux_iostreams.hpp>
#endif

#include <kickstart/core/language/Tag_.hpp>
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/core/stdlib-extensions/limits.hpp>      // bits_per_
#include <kickstart/core/stdlib-extensions/strings.hpp>     // spaces

#include <stdint.h>         // Unit

#include <array>
#include <bitset>
#include <type_traits>
#include <utility>          // swap

namespace kickstart::tag {
    using kickstart::language::Tag_;
    using From_parts        = Tag_<struct Struct_from_parts>;
    using Uninitialized     = Tag_<struct Struct_uninitialized>;
}  // namespace kickstart::tag

namespace kickstart::language::_definitions {
    using namespace kickstart::strings;
    using   kickstart::limits::bits_per_;
    using   std::array,
            std::bitset,
            std::make_signed_t,
            std::swap;

    template< class Unsigned >
    inline auto msb_is_set_in( const Unsigned value )
        -> Truth
    { return make_signed_t<Unsigned>( value ) < 0; }

    template< class Unsigned >
    inline auto lsb_is_set_in( const Unsigned value )
        -> Truth
    { return !!(value & 1); }


    class Limited_unsigned_int;
    struct Divmod_result;

    class Limited_unsigned_int
    {
    public:
        using Unit = uint8_t;
        using Parts = array<Unit, 2>;           // // 0 => lsp, 1 => msp.
        static constexpr int n_bits = 2*bits_per_<Unit>;

    private:
        using Self = Limited_unsigned_int;
        friend auto divmod( const Self& a, const Unit b ) -> Divmod_result;

        Parts   m_parts;

    public:
        Limited_unsigned_int():
            m_parts()
        {}

        Limited_unsigned_int( tag::Uninitialized )
        {}

        Limited_unsigned_int( const Unit value ):
            m_parts{ value, 0 }
        {}

        Limited_unsigned_int( tag::From_parts, const Unit lsp, const Unit msp ):
            m_parts{ lsp, msp }
        {}

        auto parts() const
            -> const Parts&
        { return m_parts; }

        auto to_bitset() const
            -> bitset<n_bits>
        {
            auto bits = bitset<n_bits>( m_parts[0] );
            for( int i = 0; i < bits_per_<Unit>; ++i ) {
                const int bit = (m_parts[1] >> i) & 1;
                bits.set( i + bits_per_<Unit>, !!bit );
            }
            return bits;
        }

        auto modulo_unit() const
            -> Unit
        { return m_parts[0]; }

        auto is_in_unit_range() const
            -> Truth
        { return m_parts[1] == 0; }

        auto operator~() const -> Self
        {
            Self result = *this;
            for( auto& part: result.m_parts ) { part = ~part; }
            return result;
        }

        void operator++()
        {
            ++m_parts[0];
            if( m_parts[0] == 0 ) {
                ++m_parts[1];
            }
        }

        void operator--()
        {
            if( m_parts[0] == 0 ) {
                --m_parts[1];
            }
            --m_parts[0];
        }

        auto operator+() const -> Self
        {
            return *this;
        }

        auto operator-() const -> Self
        {
            Self result = ~*this;
            ++result;
            return result;
        }

        void operator+=( const Self& other )
        {
            m_parts[0] += other.m_parts[0];
            const Truth carry = (m_parts[0] < other.m_parts[0]);
            m_parts[1] += Unit( other.m_parts[1] + +carry );
        }

        friend
        auto operator+( const Self& a, const Self& b ) -> Self
        {
            Self result = a;
            result += b;
            return result;
        }

        void operator-=( const Self& other )
        {
            *this += -other;
        }

        friend
        auto operator-( const Self& a, const Self& b ) -> Self
        {
            return a + -b;
        }

        void shift_left()
        {
            const Truth carry = msb_is_set_in( m_parts[0] );
            m_parts[0] <<= 1;
            m_parts[1] <<= 1;
            m_parts[1] |= +carry;
        }

        void shift_right()
        {
            const Truth carry = lsb_is_set_in( m_parts[1] );
            m_parts[1] >>= 1;
            m_parts[0] >>= 1;
            m_parts[0] |= (Unit( +carry ) << (bits_per_<Unit> - 1));
        }

        static
        auto mul_units( const Unit a, const Unit b ) -> Self
        {
            //const array<uint32_t, 2>    parts_a = { uint32_t( a & 0xFFFFFFFF ), uint32_t( a >> 32 ) };
            //const array<uint32_t, 2>    parts_b = { uint32_t( b & 0xFFFFFFFF ), uint32_t( b >> 32 ) };

            //constexpr auto one = Unit( 1 );
            //const Unit  low         = one * parts_a[0] * parts_b[0];
            //const Unit  mid_1       = one * parts_a[0] * parts_b[1];
            //const Unit  mid_2       = one * parts_a[1] * parts_b[0];
            //const Unit  mid         = mid_1 + mid_2;
            //const Truth     mid_carry   = (mid < mid_1);
            //const Unit  high        = one * parts_a[1] * parts_b[1];

            //auto result = Self( tag::Uninitialized() );
            //result.m_parts[0] = low + ((mid & 0xFFFFFFFF) << 32);
            //const Truth part_0_carry = (result.m_parts[0] < low);
            //result.m_parts[1] = (+part_0_carry) + (mid >> 32) + (Unit(+mid_carry) << 32) + high;
            //return result;
            return Self( tag::From_parts(), Unit( a*b % 256 ), Unit( a*b / 256 ) );
        }

        friend
        auto operator*( const Unit a, const Self& b ) -> Self
        {
            Self result = mul_units( b.m_parts[0], a );
            result.m_parts[1] += b.m_parts[1]*a;
            return result;
        }

        friend
        auto operator/( const Self& a, const Unit b ) -> Self
        {
            #ifndef KS_TEST_DIVISION_PLEASE
                if( a.m_parts[1] == 0 ) {
                    return a.m_parts[0]/b;
                }
            #endif

            Unit divisor = b;
            int n_shifts = 0;
            while( not msb_is_set_in( divisor ) ) {
                divisor <<= 1;
                ++n_shifts;
            }

            const int n_q_digits = n_shifts + 64;
            Self dividend   = a;
            Self result     = 0;
            for( int i = 0; i < n_q_digits; ++i ) {
                result.shift_left();
                if( divisor <= dividend.m_parts[1] ) {
                    dividend.m_parts[1] -= divisor;
                    result.m_parts[0] |= 1;
                }
                dividend.shift_left();
            }
            return result;
        }
    };

    using Luint = Limited_unsigned_int;
    struct Divmod_result
    {
        Luint   remainder;
        Luint   quotient;
    };

    inline auto divmod( const Luint& a, const Luint::Unit b )
        -> Divmod_result
    {
        using namespace faux_iostreams;
        #ifndef KS_TEST_DIVISION_PLEASE
            if( a.m_parts[1] == 0 ) {
                return {Luint::Unit( a.m_parts[0]%b ), Luint::Unit( a.m_parts[0]/b )};
            }
        #endif

        using Unit = Luint::Unit;

        Unit divisor = b;
        int n_shifts = 0;
        while( not msb_is_set_in( divisor ) ) {
            divisor <<= 1;
            ++n_shifts;
        }

        const int n_q_digits = 1 + n_shifts + bits_per_<Unit>;
        const int n_divisor_digits = bits_per_<Unit> - n_shifts;
        out << "n_shifts=" << n_shifts << ", n_q_digits = " << n_q_digits << endl;
        Divmod_result result = {a, 0};
        Truth carry = false;
        for( int i = 0; i < n_q_digits; ++i ) {
            result.quotient.shift_left();
            out << +carry << " " << spaces( i ) << result.remainder.to_bitset().to_string().substr( 0, 2*bits_per_<Unit> - i );
            if( carry or divisor <= result.remainder.m_parts[1] ) {
                result.remainder.m_parts[1] -= divisor;
                result.quotient.m_parts[0] |= 1;
            }
            out << "  " << result.quotient.to_bitset().to_string() << endl;
            out << "  " << spaces( i ) << bitset<bits_per_<Unit>>( divisor ).to_string().substr( 0, n_divisor_digits ) << endl;
            out << endl;
            carry = msb_is_set_in( result.remainder.parts()[1] );
            result.remainder.shift_left();
        }
        assert( result.remainder.m_parts[0] == 0 );
        result.remainder.m_parts[1] >>= n_q_digits;
        swap( result.remainder.m_parts[0], result.remainder.m_parts[1] );
        return result;
    }

    inline auto compare( const Luint::Unit a, const Luint::Unit b )
        -> int
    { return (a < b? -1 : a == b? 0 : +1); }


    inline auto compare( const Luint& a, const Luint& b )
        -> int
    {
        if( const int r = compare( a.parts()[1], b.parts()[1] ) ) {
            return r;
        }
        return compare( a.parts()[0], b.parts()[0] );
    }

    inline auto operator<( const Luint& a, const Luint& b )
        -> Truth
    { return (compare( a, b ) < 0); }

    inline auto operator<=( const Luint& a, const Luint& b )
        -> Truth
    { return (compare( a, b ) <= 0); }

    inline auto operator==( const Luint& a, const Luint& b )
        -> Truth
    { return (compare( a, b ) == 0); }

    inline auto operator>=( const Luint& a, const Luint& b )
        -> Truth
    { return (compare( a, b ) >= 0); }

    inline auto operator>( const Luint& a, const Luint& b )
        -> Truth
    { return (compare( a, b ) > 0); }

    inline auto operator!=( const Luint& a, const Luint& b )
        -> Truth
    { return (compare( a, b ) != 0); }

}  // namespace kickstart::language::_definitions
