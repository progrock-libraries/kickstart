// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once

// kickstart.core.hpp - minimal convenience functionality for C++ learners.
// The “core” is because the (rather large) iostreams headers are not included.
//
// Requires C++17 or later.

// Copyright (c) 2020 Alf P. Steinbach. MIT license, with license text:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Library includes actually used in this header.
#include "core/ascii.hpp"
#include "core/failure-handling.hpp"    // hopefully, fail, ...
#include "core/stdstuff.hpp"            // Safe-with-`using` stuff from std library.
#include "core/type_aliases.hpp"
#include "core/utf8.hpp"                // Workarounds for Windows.

#include <assert.h>         // assert

#include <stddef.h>         // size_t
#include <stdlib.h>         // EXIT_..., strtod
#include <string.h>         // strerror

#include <sstream>          // TODO: get rid of?

namespace kickstart::_definitions {
    using namespace kickstart::utf8::io;
    using namespace std::string_literals;
    using   std::invalid_argument, std::out_of_range;
    using   std::ostringstream;
    using   std::function;
    using   std::pair;
    using   std::getline, std::stoi, std::string;
    using   std::string_view;
    using   std::optional;
    using   std::vector;


    //----------------------------------------------------------- Misc basic stuff:

    [[noreturn]] inline void ub_here() { throw; }

    //----------------------------------------------------------- Conversion anything → text:

    namespace impl {
        inline auto as_string_append_argument( const C_str s ) -> C_str { return s; }
        inline auto as_string_append_argument( const string& s ) -> const string& { return s; }
        inline auto as_string_append_argument( const string_view& s ) -> const string_view& { return s; }

        template< class T >
        inline auto as_string_append_argument( const T& value )
            -> string
        {
            ostringstream stream;
            stream << value;
            return stream.str();
        }

        inline auto as_string_append_argument( const bool value )
            -> string
        { return (value? "T" : "F"); }
    }  // namespace impl

    template< class T >
    inline auto str( T const& value )
        -> string
    { return impl::as_string_append_argument( value ); }

    template< class T >
    inline auto operator<<( string& s, T const& value )
        -> string&
    { return s.append( impl::as_string_append_argument( value ) ); }

    template< class T >
    inline auto operator<<( string&& s, T const& value )
        -> string&&
    { return move( s << value ); }

    template< class... Args >
    inline auto concatenate( const Args&... args )
        -> string
    { return (std::string() << ... << args); }
    
    inline auto p_start_of( const string_view& s ) -> const char* { return s.data(); }
    inline auto p_end_of( const string_view& s ) -> const char* { return s.data() + s.size(); }

    inline auto trimmed_view( const string_view& s )
        -> string_view
    {
        const char* p_first = p_start_of( s );
        const char* p_beyond = p_end_of( s );
        while( p_first != p_beyond and is_( ascii::space, *p_first ) ) {
            ++p_first;
        }
        while( p_beyond != p_first and is_( ascii::space, *(p_beyond - 1) ) ) {
            --p_beyond;
        }
        return {p_first, static_cast<size_t>( p_beyond - p_first )};
    }

    inline auto trimmed_string( const string_view& s )
        -> string
    { return string( trimmed_view( s ) ); }

    inline auto trimmed( const string_view& s )
        -> string_view
    { return trimmed_view( s ); }

    inline auto trimmed( const string& s )
        -> string
    { return trimmed_string( s ); }


    //----------------------------------------------------------- Conversion text → number:

    namespace impl {
        // As of 2020 not all compilers implement C++17 std::from_chars for type double, so using strtod.
        inline auto wrapped_strtod( const C_str spec )
            -> pair<double, const char*>
        {
            char* p_end;
            errno = 0;
            const double value = strtod( spec, &p_end );
            return {value, p_end};
        }
    }  // namespace impl

    // Due to implementation via strtod the string referenced by spec must guarantee that strtod stops
    // scanning at some point, e.g. due to null-termination or invalid-as-number characters. Also, the
    // specification should not be followed by text that would be a valid continuation (e.g. digits).
    inline auto fast_full_string_to_double( const string_view& spec )
        -> double
    {
        hopefully( spec.length() != 0 )
            or KS_FAIL_( invalid_argument, "An empty string is not a valid number specification." );

        const auto [value, p_end] = impl::wrapped_strtod( p_start_of( spec ) );

        hopefully( errno != ERANGE )
            or KS_FAIL_( out_of_range, "“"s << spec << "” denotes a too large or small number." );

        hopefully( p_end >= p_end_of( spec ) )
            or KS_FAIL_( invalid_argument, "“"s << spec << "” is not a valid number specification." );

        hopefully( p_end == p_end_of( spec ) )
            or KS_FAIL_( invalid_argument, "“"s << spec << "” is followed by a valid spec continuation." );

        hopefully( errno == 0 )
            or KS_FAIL( ""s
                << "strtod(\"" << spec << "\")"
                << " unexpectedly failed with strerror message “" << strerror( errno ) << "”."
                );

        return value;
    }

    inline auto fast_trimmed_string_to_double( const string_view& spec )
        -> double
    { return fast_full_string_to_double( trimmed( spec ) ); }

    inline auto safe_full_string_to_double( const string_view& spec )
        -> double
    { return fast_full_string_to_double( string( spec ) ); }

    inline auto safe_trimmed_string_to_double( const string_view& spec )
        -> double
    { return safe_full_string_to_double( trimmed( spec ) ); }

    inline const auto& to_double = safe_trimmed_string_to_double;

    namespace impl {
        inline auto wrapped_stoi( const string& s, size_t& n_chars )
            -> int
        {
            try {
                return stoi( s, &n_chars );
            } catch( const invalid_argument& ) {
                KS_FAIL_( invalid_argument, "“" + s + "” is an invalid `int` value spec." );
            } catch( const out_of_range& ) {
                KS_FAIL_( out_of_range, "The number “" + s + "” is out of range for `int`." );
            }
            ub_here();      // Suppresses sillywarning with Visual C++.
        }
    }

    inline auto to_int( const string& s )
        -> int
    {
        size_t n_chars;
        const int result = impl::wrapped_stoi( s, n_chars );
        hopefully( n_chars == s.length() )
            or KS_FAIL( "Extraneous characters at the end of “" + s + "”." );
        return result;
    }

    inline auto to_int( const string_view& s )
        -> int
    { return to_int( string( s ) ); }

    inline auto to_int( const C_str s )
        -> int
    { return to_int( string( s ) ); }


    //----------------------------------------------------------- Program startup support:

    using Simple_startup        = void();
    using Startup_with_args     = void( const string_view&, const vector<string_view>& );

    inline auto with_exceptions_displayed( const function<Simple_startup>& do_things )
        -> int
    {
        try{
            do_things();
            return EXIT_SUCCESS;
        } catch( const Clean_app_exit_exception& x ) {
            output_error_message( ""s << x.what() << "\n" );
        } catch( const exception& x ) {
            output_error_message( "!"s << x.what() << "\n" );
        }
        return EXIT_FAILURE;
    }

    inline auto with_exceptions_displayed(
        const function<Startup_with_args>&      do_things,
        const int                               n_cmd_parts,
        const Type_<const C_str*>               cmd_parts
        ) -> int
    {
        assert( n_cmd_parts >= 1 );
        assert( cmd_parts != nullptr );
        return with_exceptions_displayed( [&]() -> void
            {
                do_things(
                    cmd_parts[0],
                    vector<string_view>( cmd_parts + 1, cmd_parts + n_cmd_parts )
                );
            } );
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::ub_here,
        d::str, d::operator<<, d::concatenate,
        d::p_start_of, d::p_end_of,
        d::trimmed, d::trimmed_string,
        d::fast_full_string_to_double, d::fast_trimmed_string_to_double,
        d::safe_full_string_to_double, d::safe_trimmed_string_to_double,
        d::to_double, d::to_int,
        d::output_to, d::output, d::any_input_from, d::input_from, d::any_input, d::input,
        d::with_exceptions_displayed;
    }  // namespace exported names
}  // namespace kickstart::_definitions

namespace kickstart::all {
    using namespace _definitions::exported_names;
}  // namespace kickstart::all
