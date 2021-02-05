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

#include <kickstart/core/collection-util.hpp>                   // begin_ptr_of, end_ptr_of
#include <kickstart/core/failure-handling.hpp>
#include <kickstart/core/stdlib-extensions/limits.hpp>          // largest_exact_integer_of_
#include <kickstart/core/text-encoding-ascii.hpp>
#include <kickstart/core/language/type-aliases.hpp>             // C_str
#include <kickstart/core/text-conversion/to-text.hpp>
#include <kickstart/core/text-conversion/to-number-exceptions.hpp>

#include <string.h>                             // strerror
#include <limits.h>                             // INT_MIN, INT_MAX

#include <algorithm>        // std::min, std::max
#include <string>
#include <string_view>
#include <utility>

namespace kickstart::text_conversion::_definitions {
    using namespace kickstart::failure_handling;
    using namespace kickstart::language;        // C_str, begin_ptr_of, end_ptr_of, lar...
    using namespace exception;                  // Invalid_argument, Out_of_range, ...
    namespace ascii = kickstart::ascii;
    using   std::min, std::max,
            std::string,
            std::string_view,
            std::pair;

    template< class Number >
    auto to_(
        const string_view&          s,
        const Type_<const char**>   pp_beyond_spec = nullptr
        ) -> Number;

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


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;

    namespace exported_names {
        namespace fast = d::fast;       // full_string_to_double, trimmed_string_to_double
        namespace safe = d::safe;       // full_string_to_double, trimmed_string_to_double
        using
            d::to_,
            d::to_double,
            d::to_int;
    }  // namespace exported names
}  // namespace kickstart::text_conversion::_definitions

namespace kickstart::text_conversion    { using namespace _definitions::exported_names; }
namespace kickstart::core               { using namespace text_conversion; }
