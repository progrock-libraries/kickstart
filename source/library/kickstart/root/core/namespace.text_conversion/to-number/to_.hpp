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

#include <kickstart/root/core/namespace.collection_utility.hpp>                 // begin_ptr_of, end_ptr_of
#include <kickstart/root/core/failure-handling.hpp>
#include <kickstart/root/core/namespace.language/types/type-aliases.hpp>        // C_str
#include <kickstart/root/core/namespace.language/operations/bits_per_.hpp>      // bits_per_
#include <kickstart/root/core/stdlib-extensions/collections/Array_span_.hpp>    // Array_span_
#include <kickstart/root/core/stdlib-extensions/basic_type_properties/floating-point-properties.hpp>    // largest_exact_integer_of_
#include <kickstart/root/core/stdlib-extensions/strings.hpp>                    // split_on_whitespace
#include <kickstart/root/core/namespace.text_conversion/to-text.hpp>
#include <kickstart/root/core/namespace.text_conversion/to-number-exceptions.hpp>
#include <kickstart/root/core/text-encoding/namespace.ascii.hpp>

#include <kickstart/c/string.hpp>   // strerror
#include <kickstart/c/limits.hpp>   // INT_MIN, INT_MAX

#include <algorithm>        // std::min, std::max
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace kickstart::text_conversion::_definitions {
    using namespace kickstart::collection_utility;          // begin_ptr_of, end_ptr_of
    using namespace kickstart::failure_handling;
    using namespace kickstart::language;                    // C_str
    using namespace kickstart::basic_type_properties;       // largest_exact_integer_of_
    using namespace kickstart::text_conversion::exception;  // Invalid_argument, Out_of_range, ...

    namespace ascii = kickstart::ascii;

    using   kickstart::collections::Array_span_;
    using   kickstart::strings::split_on_whitespace;
    using   kickstart::language::lx::bits_per_;
    using   std::min, std::max,                         // <algorithm>
            std::string,                                // <string>
            std::string_view,                           // <string_view>
            std::is_integral_v, std::make_unsigned_t,   // <type_traits>
            std::enable_if_t, std::pair,                // <utility>
            std::vector;                                // <vector>

    template< class Number >
    auto to_(
        const string_view&          s,
        const Type_<const char**>   pp_beyond_spec = nullptr
        ) -> Number;

    template< class Number >
    auto to_vector_( const Array_span_<const string_view>& strings )
        -> vector<Number>
    {
        vector<Number> result;
        result.reserve( strings.size() );
        for( const string_view& s : strings ) {
            result.push_back( to_<Number>( s ) );
        }
        return result;
    }

    template< class Number >
    auto parts_to_vector_( const string_view& s )
        -> vector<Number>
    { return to_vector_<Number>( split_on_whitespace( s ) ); }

    // As of 2020 not all compilers implement C++17 std::from_chars for type double, so using strtod.
    inline auto wrapped_strtod( const C_str spec ) noexcept
        -> pair<double, const char*>
    {
        char* p_end;
        errno = 0;
        const double value = strtod( spec, &p_end );
        return {value, p_end};
    }

    namespace fast {
        // Due to implementation via strtod the string referenced by spec must guarantee that strtod stops
        // scanning at some point, e.g. due to null-termination or invalid-as-number characters. The
        // specification should not be followed by text that would be a valid continuation (e.g. digits).
        // The LC_NUMERIC category setting of the current C library locale determines recognition of
        // the radix point character, essentially English period or mainland European comma.
        //
        inline auto full_string_to_double(
            const string_view&          spec,
            const Type_<const char**>   pp_beyond_spec = nullptr
            ) -> double
        {
            hopefully( spec.length() != 0 )
                or KS_FAIL_( Empty_specification, "An empty string is not a valid number specification." );

            const auto [value, p_end] = wrapped_strtod( begin_ptr_of( spec ) );
            if( pp_beyond_spec ) {
                *pp_beyond_spec = p_end;
            }

            hopefully( errno != ERANGE )
                or KS_FAIL_( Representable_range_exceeded, "“"s << spec << "” denotes a too large or small number." );

            hopefully( not( p_end == begin_ptr_of( spec ) ) )
                or KS_FAIL_( Invalid_argument, "“"s << spec << "” is not a valid number specification." );

            hopefully( not( p_end < end_ptr_of( spec ) ) )
                or KS_FAIL_( Unexpected_suffix_text, "“"s << spec << "” has some unexpected text at the end." );

            hopefully( p_end == end_ptr_of( spec ) )
                or KS_FAIL_( Unexpected_spec_extension, "“"s << spec << "” is followed by a valid spec continuation." );

            hopefully( errno == 0 )
                or KS_FAIL( ""s
                    << "strtod(\"" << spec << "\")"
                    << " unexpectedly failed with strerror message “" << strerror( errno ) << "”."
                );

            return value;
        }

        inline auto trimmed_string_to_double(
            const string_view&          spec,
            const Type_<const char**>   pp_beyond_spec = nullptr
            ) -> double
        { return full_string_to_double( ascii::trimmed( spec ), pp_beyond_spec ); }
    }  // namespace fast

    namespace safe {
        inline auto full_string_to_double(
            const string_view&          spec,
            const Type_<const char**>   pp_beyond_spec = nullptr
            ) -> double
        { return fast::full_string_to_double( string( spec ), pp_beyond_spec ); }

        inline auto trimmed_string_to_double(
            const string_view&          spec,
            const Type_<const char**>   pp_beyond_spec = nullptr
            ) -> double
        { return full_string_to_double( ascii::trimmed( spec ), pp_beyond_spec ); }
    }  // namespace safe

    template<>
    inline auto to_<double>(
        const string_view&          s,
        const Type_<const char**>   pp_beyond_spec
        ) -> double
    { return safe::trimmed_string_to_double( s, pp_beyond_spec ); }

    [[deprecated]]
    inline auto to_double( const string_view& s )
        -> double
    { return safe::trimmed_string_to_double( s ); }

    template<>
    inline auto to_<int>(
        const string_view&          s,
        const Type_<const char**>   pp_beyond_spec
        ) -> int
    {
        Type_<const char*> p_beyond_spec;
        const double d = to_<double>( s, &p_beyond_spec );
        if( pp_beyond_spec ) {
            *pp_beyond_spec = p_beyond_spec;
        }
        hopefully( -largest_exact_integer_of_<double> <= d and d <= largest_exact_integer_of_<double> )
            or KS_FAIL_( exception::Out_of_range,
                "This integer value could not be represented exactly with type `<double>`."
                );
        hopefully( INT_MIN <= d and d <= INT_MAX )
            or KS_FAIL_( exception::Representable_range_exceeded,
                "The representable value range for type `int` was exceeded."
                );
        const int result = int( d );
        hopefully( result == d )
            or KS_FAIL_( exception::Decimals_specified_for_integer,
                "Decimals were specified for an integer value."
                );
        return result;
    }

    [[deprecated]]
    inline auto to_int( const string& s )
        -> int
    { return to_<int>( s ); }

    [[deprecated]]
    inline auto to_int( const string_view& s )
        -> int
    { return to_<int>( s ); }

    [[deprecated]]
    inline auto to_int( const C_str s )
        -> int
    { return to_<int>( s ); }

    template<
        class Integer,
        class = enable_if_t< is_integral_v<Integer> >       // TODO: Add overload for enumerations.
        >
    inline auto to_hex( const Integer value, const string_view& hex_digits = "0123456789ABCDEF" )
        -> string
    {
        static_assert( bits_per_<char> == 8 );
        using Unsigned = make_unsigned_t<Integer>;

        assert( hex_digits.length() >= 16 );
        const int n_bytes = sizeof( Integer );
        const int n_digits = 2*n_bytes;
        auto result = string( n_digits, '\0' );
        Unsigned x = value;
        for( int i = n_digits; i --> 0; ) {
            const unsigned digit = x & 0xF;
            result[i] = hex_digits[digit];
            x >>= 4;
        }
        return result;
    }

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;

    namespace exported_names {
        namespace fast = d::fast;       // full_string_to_double, trimmed_string_to_double
        namespace safe = d::safe;       // full_string_to_double, trimmed_string_to_double
        using
            d::to_,
            d::to_vector_,
            d::parts_to_vector_,
            d::to_double,
            d::to_int,
            d::to_hex;
    }  // namespace exported names
}  // namespace kickstart::text_conversion::_definitions

namespace kickstart::text_conversion    { using namespace _definitions::exported_names; }
