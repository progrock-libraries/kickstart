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

#include "../type_aliases.hpp"          // C_str

#include <sstream>
#include <string>
#include <string_view>

namespace kickstart::text_conversion::_definitions {
    using namespace kickstart::type_aliases;        // C_str etc.
    using   std::ostringstream,
            std::string,
            std::string_view;

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


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names {
        using namespace std::string_literals;
        using namespace std::string_view_literals;
        using
            d::str,
            d::operator<<,
            d::concatenate;
    }  // namespace exported names
}  // namespace kickstart::text_conversion::_definitions

namespace kickstart::text_conversion    { using namespace _definitions::exported_names; }
namespace kickstart::all                { using namespace text_conversion; }
