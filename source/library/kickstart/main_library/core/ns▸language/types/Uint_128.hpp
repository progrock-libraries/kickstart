﻿// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
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

#include <kickstart/main_library/core/ns▸collection_support/collection-sizes.hpp>                          // int_size
#include <kickstart/main_library/core/ns▸generate/Relational_operators_from_compare_.hpp>  // Relational_operators_from_compare_
#include <kickstart/main_library/core/ns▸language/syntax-support/KS_STATIC_ASSERT.hpp>                             // KS_STATIC_ASSERT
#include <kickstart/main_library/core/ns▸language/parameters-support/Tag_.hpp>                                     // Tag_
#include <kickstart/main_library/core/ns▸language/types/Truth.hpp>                                         // Truth
#include <kickstart/main_library/core/ns▸language/types/type-aliases.hpp>                                  // C_str
#include <kickstart/main_library/core/ns▸language/types/Uint_double_of_.hpp>                               // Uint_double_of_
#include <kickstart/main_library/core/stdlib-extensions/ns▸basic_type_properties/integer-properties.hpp>   // bits_per_
#include <kickstart/main_library/core/stdlib-extensions//ns▸strings.hpp>                                   // spaces
#include <kickstart/main_library/core/ns▸text_conversion/to-text/string-output-operator.hpp>

#include <kickstart/c/assert.hpp>       // assert
#include <kickstart/c/stdint.hpp>       // uint64_t

#include <algorithm>        // min
#include <array>
#include <bitset>
#include <optional>
#include <stdexcept>        // runtime_error
#include <string>           // string
#include <string_view>      // string_view
#include <type_traits>      // is_integral_v

namespace kickstart::tag {
    using kickstart::language::Tag_;
    using From_parts        = Tag_<struct Struct_from_value>;
    using Uninitialized     = Tag_<struct Struct_uninitialized>;
}  // namespace kickstart::tag

namespace kickstart::language::_definitions {
    using namespace kickstart::strings;
    using namespace kickstart::text_conversion;     // string <<
    
    namespace kl = kickstart::language;

    using   kl::C_str, kl::Truth;
    using   kickstart::collection_support::int_size;
    using   kickstart::basic_type_properties::bits_per_;
    using   std::min,
            std::array,
            std::bitset,
            std::optional,
            std::runtime_error,
            std::string,
            std::string_view,
            std::is_integral_v;

    class Uint_128:
        public generate::Relational_operators_from_compare_<Uint_128>
    {
    public:
        using Part      = uint64_t;
        using Parts     = Uint_double_of_<Part>;
        static constexpr int n_bits = 2*bits_per_<Part>;

    private:
        using Self = Uint_128;

        Parts   m_value;

    public:
        Uint_128( tag::Uninitialized ) {}
        constexpr Uint_128(): m_value() {}
        constexpr Uint_128( const Parts& parts ): m_value( parts ) {}
        constexpr Uint_128( tag::From_parts, const Part lsp, const Part msp = 0 ):m_value{ lsp, msp } {}

        // Yields `value` modulo 2^128.
        template< class Integer >
        constexpr Uint_128( const Integer value ):
            m_value{ Part( value ), 0 }
        {
            KS_STATIC_ASSERT( is_integral_v<Integer> );
            if constexpr( sizeof( Integer ) > sizeof( Part ) ) {
                m_value.parts[1] = Part( value >> bits_per_<Part> );
            } else if( value < 0 ) {
                m_value.parts[1] = Part( -1 );      // Sign extension.
            }
        }

        template< int set_size >
        Uint_128( const bitset<set_size>& bits ):
            m_value()
        {
            const int n = min<int>( n_bits, set_size );
            for( int i_bit = 0; i_bit < n ; ++i_bit ) {
                if( bits[i_bit] ) {
                    const int i_part = i_bit / bits_per_<Part>;
                    m_value.parts[i_part] |= (1ULL << (i_bit % bits_per_<Part>));
                }
            }
        }

        constexpr Uint_128( const Self& ) = default;
        auto constexpr operator=( const Self& ) -> Self& = default;

        inline constexpr auto representation() const -> const Parts&;
        inline auto to_bitset() const -> bitset<n_bits>;
        inline constexpr auto modulo_64_bits() const -> Part;
        inline constexpr auto is_in_64_bit_range() const -> Truth;

        inline constexpr void operator++();
        inline constexpr void operator--();

        inline constexpr void operator+=( const Self& other );
        inline constexpr void operator-=( const Self& other );
        //inline constexpr void operator*=( const Part a );
        //inline constexpr void operator/=( const Part a );

        inline constexpr auto operator~() const -> Self;
        inline constexpr void shift_left();
        inline constexpr void shift_right();

        struct Divmod_result;
        inline constexpr auto divmod_by_64_bit( const Part b ) const -> Divmod_result;

        struct Result_kind{ enum Enum{ math_exact, wrapped }; };
        inline constexpr auto add_64_bit( const Part a ) -> Result_kind::Enum;
        inline constexpr auto subtract_64_bit( const Part a ) -> Result_kind::Enum;
        inline constexpr auto multiply_by_64_bit( const Part a ) -> Result_kind::Enum;
        inline constexpr auto divide_by_64_bit( const Part a ) -> Result_kind::Enum;
        inline constexpr auto mod_by_64_bit( const Part a ) -> Result_kind::Enum;

        inline constexpr auto add( const Self& a ) -> Result_kind::Enum;
        inline constexpr auto subtract( const Self& other ) -> Result_kind::Enum;
    };

    inline constexpr auto operator+( const Uint_128& ) -> Uint_128;
    inline constexpr auto operator-( const Uint_128& ) -> Uint_128;

    inline constexpr auto operator+( const Uint_128& a, const Uint_128& b ) -> Uint_128;
    inline constexpr auto operator-( const Uint_128& a, const Uint_128& b ) -> Uint_128;

    inline constexpr auto operator*( const Uint_128::Part a, const Uint_128& b ) -> Uint_128;
    inline constexpr auto operator/( const Uint_128& a, const Uint_128::Part b ) -> Uint_128;
    inline constexpr auto operator%( const Uint_128& a, const Uint_128::Part b ) -> Uint_128;

    inline constexpr auto compare( const Uint_128& a, const Uint_128& b ) -> int;

    inline auto str( const Uint_128& v ) -> string;


    //--------------------------------------------------------------------------------------------------

    inline constexpr auto Uint_128::representation() const
        -> const Parts&
    { return m_value; }

    inline auto Uint_128::to_bitset() const
        -> bitset<n_bits>
    {
        using B = bitset<n_bits>;
        return (B( m_value.parts[1] ) << bits_per_<Part>) | B( m_value.parts[0] );
    }

    inline constexpr auto Uint_128::modulo_64_bits() const
        -> Part
    { return m_value.parts[0]; }

    inline constexpr auto Uint_128::is_in_64_bit_range() const
        -> Truth
    { return m_value.parts[1] == 0; }

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
        m_value.parts[1] += Part( other.m_value.parts[1] + +carry );
    }

    inline constexpr void Uint_128::operator-=( const Self& other )
    {
        *this += -other;
    }

    inline constexpr auto Uint_128::operator~() const
        -> Self
    { return Uint_128( ~m_value ); }

    inline constexpr void Uint_128::shift_left()
    {
        m_value.shift_left();
    }

    inline constexpr void Uint_128::shift_right()
    {
        m_value.shift_right();
    }

    struct Uint_128::Divmod_result
    {
        Uint_128   remainder;
        Uint_128   quotient;
    };

    inline constexpr auto Uint_128::divmod_by_64_bit( const Uint_128::Part b ) const
        -> Divmod_result
    {
        const Parts::Divmod_result internal_states = Parts::quotient_of( m_value, b );
        return Divmod_result{ internal_states.remainder, internal_states.quotient };
    }

    inline constexpr auto Uint_128::add_64_bit( const Part a )
        -> Result_kind::Enum
    {
        using R = Result_kind;
        m_value.parts[0] += a;
        const Truth carry = (m_value.parts[0] < a);
        m_value.parts[1] += +carry;
        return (m_value.parts[1] == 0 and m_value.parts[0] < a? R::wrapped : R::math_exact);
    }

    inline constexpr auto Uint_128::subtract_64_bit( const Part a )
        -> Result_kind::Enum
    { return subtract( Self( a ) ); }

    inline constexpr auto Uint_128::multiply_by_64_bit( const Part a )
        -> Result_kind::Enum
    {
        const Parts lo  = Parts::product_of( a, m_value.parts[0] );
        const Parts hi  = Parts::product_of( a, m_value.parts[1] );
        const Truth intermediate_overflow = (hi.parts[1] != 0);
        m_value = { lo.parts[0], lo.parts[1] + hi.parts[0] };
        const Truth final_overflow = (m_value.parts[1] < lo.parts[1]);
        using R = Result_kind;
        return (intermediate_overflow or final_overflow? R::wrapped : R::math_exact);
    }

    inline constexpr auto Uint_128::divide_by_64_bit( const Part a )
        -> Result_kind::Enum
    {
        *this = divmod_by_64_bit( a ).quotient;
        return Result_kind::math_exact;
    }

    inline constexpr auto Uint_128::mod_by_64_bit( const Part a )
        -> Result_kind::Enum
    {
        *this = divmod_by_64_bit( a ).remainder;
        return Result_kind::math_exact;
    }

    inline constexpr auto Uint_128::add( const Self& other )
        -> Result_kind::Enum
    {
        using R = Result_kind;
        operator+=( other );
        return (*this < other? R::wrapped : R::math_exact);
    }

    inline constexpr auto Uint_128::subtract( const Self& other )
        -> Result_kind::Enum
    {
        using R = Result_kind;
        const Truth wrapping = (other > *this);
        operator-=( other );
        return (wrapping? R::wrapped : R::math_exact);
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

    inline constexpr auto operator*( const Uint_128::Part a, const Uint_128& b )
        -> Uint_128
    {
        Uint_128 result = b;
        result.multiply_by_64_bit( a );
        return result;
    }

    inline constexpr auto operator/( const Uint_128& a, const Uint_128::Part b )
        -> Uint_128
    { return a.divmod_by_64_bit( b ).quotient; }

    inline constexpr auto operator%( const Uint_128& a, const Uint_128::Part b )
        -> Uint_128
    { return a.divmod_by_64_bit( b ).remainder; }

    inline constexpr auto compare( const Uint_128& a, const Uint_128& b )
        -> int
    { return compare( a.representation(), b.representation() ); }

    inline auto str( const Uint_128& v )
        -> string
    {
        if( v == 0 ) { return "0"; }

        string digits;
        Uint_128 a = v;
        while( a != 0 ) {
            const auto r = a.divmod_by_64_bit( 10 );
            digits += "0123456789"[r.remainder.modulo_64_bits()];
            a = r.quotient;
        }
        reverse( begin( digits ), end( digits ) );
        return digits;
    }

    const char  apostrophe  = '\'';

    inline constexpr auto to_uint_128( const string_view& spec )
        -> Uint_128
    {
        const auto ensure_is_valid = []( const char ch, const int index ) constexpr
        {
            const Truth is_valid = (('0' <= ch and ch <= '9') or (index > 0 and ch == apostrophe));
            if( not is_valid ) {
                throw runtime_error( "Invalid character ‘"s << ch << "’ in Uint_128 value spec." );
            }
        };

        using R = Uint_128::Result_kind;
        Uint_128 result = 0;
        for( int i = 0; i < int_size( spec ); ++i ) {
            const char ch = spec[i];
            ensure_is_valid( ch, i );
            if( ch != apostrophe ) {
                const R::Enum r1 = result.multiply_by_64_bit( 10 );
                const R::Enum r2 = result.add_64_bit( ch - '0' );
                if( r1 == R::wrapped or r2 == R::wrapped ) {
                    throw runtime_error( "Uint_128 value range exceeded for value spec."s );
                }
            }
        }
        return result;
    }

    inline constexpr auto operator""_u128( const C_str spec )
        -> Uint_128
    { return to_uint_128( string_view( spec ) ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Uint_128,
        d::to_uint_128,
        d::operator""_u128;
    }  // namespace exported_names
}  // namespace kickstart::language::_definitions
