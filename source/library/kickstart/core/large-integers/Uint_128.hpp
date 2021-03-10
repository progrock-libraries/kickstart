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
#include <kickstart/core/language/Truth.hpp>                // Truth
#include <kickstart/core/language/type-aliases.hpp>         // C_str
#include <kickstart/core/language/lx/bit-checking.hpp>
#include <kickstart/core/large-integers/Uint_double_of_.hpp>
#include <kickstart/core/stdlib-extensions/limits.hpp>      // bits_per_
#include <kickstart/core/stdlib-extensions/strings.hpp>     // spaces

#include <assert.h>         // assert
#include <stdint.h>         // Unit

#include <array>
#include <bitset>
#include <optional>
#include <string>
#include <utility>          // swap

namespace kickstart::tag {
    using kickstart::language::Tag_;
    using From_parts        = Tag_<struct Struct_from_value>;
    using Uninitialized     = Tag_<struct Struct_uninitialized>;
}  // namespace kickstart::tag

namespace kickstart::large_integers::_definitions {
    using namespace kickstart::strings;
    
    namespace kl = kickstart::language;
    namespace klx = kickstart::language::lx;

    using   kl::C_str, kl::Truth;
    using   klx::lsb_is_set_in, klx::msb_is_set_in;
    using   kickstart::limits::bits_per_;
    using   std::array,
            std::bitset,
            std::optional,
            std::string,
            std::swap;

    class Uint_128
    {
    public:
        using Half_unit = uint32_t;
        using Unit      = uint64_t;
        using Parts     = Uint_double_of_<Unit>;
        static constexpr int n_bits = 2*bits_per_<Unit>;

    private:
        using Self = Uint_128;

        Parts   m_value;

    public:
        constexpr Uint_128(): m_value() {}
        Uint_128( tag::Uninitialized ) {}
        constexpr Uint_128( const Unit value ): m_value{ value, 0 } {}
        constexpr Uint_128( tag::From_parts, const Unit lsp, const Unit msp ): m_value{ lsp, msp } {}

        constexpr Uint_128( const Self& ) = default;
        auto constexpr operator=( const Self& ) -> Self& = default;

        inline constexpr auto representation() const -> const Parts&;
        inline auto to_bitset() const -> bitset<n_bits>;
        inline constexpr auto modulo_unit() const -> Unit;
        inline constexpr auto is_in_unit_range() const -> Truth;
        inline constexpr auto operator~() const -> Self;

        inline constexpr void operator++();
        inline constexpr void operator--();

        inline constexpr void operator+=( const Self& other );
        inline constexpr void operator-=( const Self& other );

        inline constexpr void shift_left();
        inline constexpr void shift_right();

        struct Divmod_result;
        inline constexpr auto divmod_unit( const Unit b ) const -> Divmod_result;
    };

    inline constexpr auto operator+( const Uint_128& ) -> Uint_128;
    inline constexpr auto operator-( const Uint_128& ) -> Uint_128;

    inline constexpr auto operator+( const Uint_128& a, const Uint_128& b ) -> Uint_128;
    inline constexpr auto operator-( const Uint_128& a, const Uint_128& b ) -> Uint_128;

    inline constexpr auto mul( const Uint_128::Unit a, const Uint_128& b ) -> optional<Uint_128>;
    inline constexpr auto operator*( const Uint_128::Unit a, const Uint_128& b ) -> Uint_128;
    inline constexpr auto operator/( const Uint_128& a, const Uint_128::Unit b ) -> Uint_128;
    inline constexpr auto operator%( const Uint_128& a, const Uint_128::Unit b ) -> Uint_128;

    inline constexpr auto compare( const Uint_128& a, const Uint_128& b ) -> int;
    inline constexpr auto operator<( const Uint_128& a, const Uint_128& b ) -> Truth;
    inline constexpr auto operator<=( const Uint_128& a, const Uint_128& b ) -> Truth;
    inline constexpr auto operator==( const Uint_128& a, const Uint_128& b ) -> Truth;
    inline constexpr auto operator>=( const Uint_128& a, const Uint_128& b ) -> Truth;
    inline constexpr auto operator>( const Uint_128& a, const Uint_128& b ) -> Truth;
    inline constexpr auto operator!=( const Uint_128& a, const Uint_128& b ) -> Truth;

    inline auto str( const Uint_128& v ) -> string;


    //--------------------------------------------------------------------------------------------------

    inline constexpr auto Uint_128::representation() const
        -> const Parts&
    { return m_value; }

    inline auto Uint_128::to_bitset() const
        -> bitset<n_bits>
    {
        auto bits = bitset<n_bits>( m_value.parts[0] );
        for( int i = 0; i < bits_per_<Unit>; ++i ) {
            const int bit = (m_value.parts[1] >> i) & 1;
            bits.set( i + bits_per_<Unit>, !!bit );
        }
        return bits;
    }

    inline constexpr auto Uint_128::modulo_unit() const
        -> Unit
    { return m_value.parts[0]; }

    inline constexpr auto Uint_128::is_in_unit_range() const
        -> Truth
    { return m_value.parts[1] == 0; }

    inline constexpr auto Uint_128::operator~() const
        -> Self
    { return Uint_128( tag::From_parts(), ~m_value.parts[0], ~m_value.parts[1] ); }

    inline constexpr void Uint_128::operator++()
    {
        ++m_value.parts[0];
        if( m_value.parts[0] == 0 ) {
            ++m_value.parts[1];
        }
    }

    inline constexpr void Uint_128::operator--()
    {
        if( m_value.parts[0] == 0 ) {
            --m_value.parts[1];
        }
        --m_value.parts[0];
    }

    inline constexpr void Uint_128::operator+=( const Self& other )
    {
        m_value.parts[0] += other.m_value.parts[0];
        const Truth carry = (m_value.parts[0] < other.m_value.parts[0]);
        m_value.parts[1] += Unit( other.m_value.parts[1] + +carry );
    }

    inline constexpr void Uint_128::operator-=( const Self& other )
    {
        *this += -other;
    }

    inline constexpr void Uint_128::shift_left()
    {
        const Truth carry = msb_is_set_in( m_value.parts[0] );
        m_value.parts[0] <<= 1;
        m_value.parts[1] <<= 1;
        m_value.parts[1] |= +carry;
    }

    inline constexpr void Uint_128::shift_right()
    {
        const Truth carry = lsb_is_set_in( m_value.parts[1] );
        m_value.parts[1] >>= 1;
        m_value.parts[0] >>= 1;
        m_value.parts[0] |= (Unit( +carry ) << (bits_per_<Unit> - 1));
    }

    struct Uint_128::Divmod_result
    {
        Uint_128   remainder;
        Uint_128   quotient;
    };

    inline constexpr auto Uint_128::divmod_unit( const Uint_128::Unit b ) const
        -> Divmod_result
    {
        if( b == 0 ) { return Divmod_result{ ~Uint_128(), ~Uint_128() }; }

        #ifndef KS_TEST_DIVISION_PLEASE
            if( m_value.parts[1] == 0 ) {
                return Divmod_result{ Unit( m_value.parts[0] % b ), Unit( m_value.parts[0] / b ) };
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
            if( carry or divisor <= result.remainder.m_value.parts[1] ) {
                result.remainder.m_value.parts[1] -= divisor;
                result.quotient.m_value.parts[0] |= 1;
            }

            ++i;
            if( i == n_q_digits ) {
                break;
            }

            result.quotient.shift_left();
            carry = msb_is_set_in( result.remainder.representation().parts[1] );
            result.remainder.shift_left();
        }
        assert( result.remainder.m_value.parts[0] == 0 );
        swap( result.remainder.m_value.parts[0], result.remainder.m_value.parts[1] );
        result.remainder.m_value.parts[0] >>= n_shifts;
        return result;
    }

    inline constexpr auto operator+( const Uint_128& value )
        -> Uint_128
    { return value; }

    inline constexpr auto operator-( const Uint_128& value )
        -> Uint_128
    {
        Uint_128 result = ~value;
        ++result;
        return result;
    }

    inline constexpr auto operator+( const Uint_128& a, const Uint_128& b )
        -> Uint_128
    {
        Uint_128 result = a;
        result += b;
        return result;
    }

    inline constexpr auto operator-( const Uint_128& a, const Uint_128& b )
        -> Uint_128
    { return a + -b; }

    inline constexpr auto mul( const Uint_128::Unit a, const Uint_128& b )
        -> optional<Uint_128>
    {
        using Unit          = Uint_128::Unit;
        using Half_unit     = Uint_128::Half_unit;

        const Uint_double_of_<Unit> lower = multiply_by_parts<Half_unit>( a, b.representation().parts[0] );
        auto result = Uint_128( tag::From_parts(),
            lower.parts[0],
            lower.parts[1] + a*b.representation().parts[1]
            );
        const Truth overflow = (result.representation().parts[1] < lower.parts[1]);
        if( overflow ) { return {}; }
        return result;
    }

    inline constexpr auto operator*( const Uint_128::Unit a, const Uint_128& b )
        -> Uint_128
    { return mul( a, b ).value_or( 0 ); }

    inline constexpr auto operator/( const Uint_128& a, const Uint_128::Unit b )
        -> Uint_128
    { return a.divmod_unit( b ).quotient; }

    inline constexpr auto operator%( const Uint_128& a, const Uint_128::Unit b )
        -> Uint_128
    { return a.divmod_unit( b ).remainder; }

    inline constexpr auto compare( const Uint_128& a, const Uint_128& b )
        -> int
    { return compare( a.representation(), b.representation() ); }

    inline constexpr auto operator<( const Uint_128& a, const Uint_128& b )
        -> Truth
    { return (compare( a, b ) < 0); }

    inline constexpr auto operator<=( const Uint_128& a, const Uint_128& b )
        -> Truth
    { return (compare( a, b ) <= 0); }

    inline constexpr auto operator==( const Uint_128& a, const Uint_128& b )
        -> Truth
    { return (compare( a, b ) == 0); }

    inline constexpr auto operator>=( const Uint_128& a, const Uint_128& b )
        -> Truth
    { return (compare( a, b ) >= 0); }

    inline constexpr auto operator>( const Uint_128& a, const Uint_128& b )
        -> Truth
    { return (compare( a, b ) > 0); }

    inline constexpr auto operator!=( const Uint_128& a, const Uint_128& b )
        -> Truth
    { return (compare( a, b ) != 0); }

    inline auto str( const Uint_128& v )
        -> string
    {
        if( v == 0 ) { return "0"; }

        string digits;
        Uint_128 a = v;
        while( a != 0 ) {
            const auto r = a.divmod_unit( 10 );
            digits += "0123456789"[r.remainder.modulo_unit()];
            a = r.quotient;
        }
        reverse( begin( digits ), end( digits ) );
        return digits;
    }

    inline constexpr auto operator""_u128( const C_str spec )
        -> Uint_128
    {
        Uint_128 result = 0;
        for( int i = 0; spec[i] != '\0'; ++i ) {
            const char ch = spec[i];
            if( ch != 0 ) {
                if( not( '0' <= ch and ch <= '9' ) ) {
                    throw 777;
                }
                result = 10*result;    // TODO: use *=.
                result += (ch - '0');
            }
        }
        return result;
    }

}  // namespace kickstart::large_integers::_definitions
