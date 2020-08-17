#pragma once
// kickstart.hpp - minimal convenience functionality for C++ learners.
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

#include <stddef.h>         // size_t
#include <stdlib.h>         // EXIT_..., strtod
#include <string.h>         // strerror

#include <functional>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <typeinfo>         // Necessary for use of typeid.

#define KS_FAIL_( X, s )    ::kickstart::fail_<X>( ::kickstart::concatenate( __func__, " - ", s ) )
#define KS_FAIL( s )        KS_FAIL_( std::runtime_error, s )

namespace kickstart {
    using namespace std::literals;

    using   std::invoke, std::function;
    using   std::cin, std::cout, std::cerr, std::endl;
    using   std::invalid_argument, std::exception, std::out_of_range, std::runtime_error;
    using   std::ostringstream;
    using   std::getline, std::string;
    using   std::string_view;
    using   std::move, std::pair;

    using C_str = const char*;

    inline auto as_string_append_argument( const C_str s ) -> const C_str { return s; }
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

    template< class T >
    inline auto operator<<( string& s, T const& value )
        -> string&
    { return s.append( as_string_append_argument( value ) ); }

    template< class T >
    inline auto operator<<( string&& s, T const& value )
        -> string&&
    { return move( s << value ); }

    template< class... Args >
    inline auto concatenate( const Args&... args )
        -> string
    { return (std::string() << ... << args); }
    
    inline auto is_ascii_space( const char ch )
        -> bool
    {
        const auto code = static_cast<unsigned char>( ch );
        return code < 128 and isspace( code );
    }

    inline auto p_start_of( const string_view& s ) -> const char* { return &*s.begin(); }
    inline auto p_end_of( const string_view& s ) -> const char* { return &*s.end(); }

    inline auto trimmed( const string_view& s )
        -> string_view
    {
        const char* p_first = p_start_of( s );
        const char* p_beyond = p_end_of( s );
        while( p_first != p_beyond and is_ascii_space( *p_first ) ) {
            ++p_first;
        }
        while( p_beyond != p_first and is_ascii_space( *(p_beyond - 1) ) ) {
            --p_beyond;
        }
        return {p_first, static_cast<size_t>( p_beyond - p_first )};
    }

    template< class X >
    inline auto fail_( const string s ) -> bool { throw runtime_error( s ); }

    inline auto hopefully( const bool condition ) -> bool { return condition; }

    // As of 2020 not all compilers implement C++17 std::from_chars for type double, so using strtod.
    inline auto wrapped_strtod( const C_str spec )
        -> pair<double, const char*>
    {
        char* p_end;
        errno = 0;
        const double value = strtod( spec, &p_end );
        return {value, p_end};
    }

    inline auto exact_string_to_double( const string_view& spec )
        -> double
    {
        hopefully( spec.length() != 0 )
            or KS_FAIL_( invalid_argument, "An empty string is not a valid number specification." );

        const auto [value, p_end] = wrapped_strtod( p_start_of( spec ) );

        hopefully( errno != ERANGE )
            or KS_FAIL_( out_of_range, "“"s << spec << "” denotes a too large or small number." );

        hopefully( p_end == p_end_of( spec ) )
            or KS_FAIL_( invalid_argument, "“"s << spec << "” is not a valid number specification." );

        hopefully( errno == 0 )
            or KS_FAIL( ""s
                << "strtod(\"" << spec << "\")"
                << " unexpectedly failed with strerror message “" << strerror( errno ) << "”."
                );

        return value;
    }

    inline auto to_double( const string_view& spec )
        -> double
    { return exact_string_to_double( trimmed( spec ) ); }

    inline auto input()
        -> string
    {
        string line;
        getline( cin, line ) or KS_FAIL( "std::getline failed" );
        return line;
    }

    inline auto input( const string_view& prompt )
        -> string
    {
        cout << prompt;
        return input();
    }
    
    inline auto with_exceptions_displayed( const function<void()>& f )
        -> int
    {
        using   std::cerr, std::endl,
                std::exception;

        try{
            f();
            return EXIT_SUCCESS;
        } catch( const exception& x ) {
            cerr << "!" << x.what() << endl;
        }
        return EXIT_FAILURE;
    }
}  // namespace kickstart
