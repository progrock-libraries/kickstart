// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../../assertion-headers/¤-assert-reasonable-compiler.hpp"

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

#include "../../language/type_aliases.hpp"      // C_str

#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

namespace kickstart::text_conversion::_definitions {
    using namespace kickstart::language;    // C_str etc.
    using   std::ostringstream,
            std::string,
            std::string_view,
            std::is_convertible_v;

    namespace impl {
        inline auto as_string_append_argument( const string_view& s ) -> const string_view& { return s; }
        inline auto as_string_append_argument( const string& s ) -> const string& { return s; }
        inline auto as_string_append_argument( const C_str s ) -> C_str { return s; }
        inline auto as_string_append_argument( const bool value ) -> string_view { return (value? "T" : "F"); }

        enum class Conversion_kind_value {  general, to_bool, to_c_str, to_string, to_string_view, none };

        template< class Type >
        constexpr auto conversion_kind_value_()
            -> Conversion_kind_value
        {
            // TODO: none-value
            if constexpr( is_convertible_v<Type, string_view> ) {
                return Conversion_kind_value::to_string_view;
            } else if constexpr( is_convertible_v<Type, string>) {
                return Conversion_kind_value::to_string;
            } else if constexpr( is_convertible_v<Type, C_str> ) {
                return Conversion_kind_value::to_c_str;
            //} else if constexpr( is_convertible_v<Type, bool> ) {     // TODO
            //    return Conversion_kind_value::to_bool;
            } else {
                return Conversion_kind_value::general;
            }
        }

        struct Conversion_kind_general {};
        struct Conversion_kind_to_bool {};
        struct Conversion_kind_to_c_str {};
        struct Conversion_kind_to_string {};
        struct Conversion_kind_to_string_view {};
        struct Conversion_kind_none {};

        template< Conversion_kind_value kind > struct Conversion_kind_t_;
        template<> struct Conversion_kind_t_<Conversion_kind_value::general>{ using T = Conversion_kind_general; };
        template<> struct Conversion_kind_t_<Conversion_kind_value::to_bool>{ using T = Conversion_kind_to_bool; };
        template<> struct Conversion_kind_t_<Conversion_kind_value::to_c_str>{ using T = Conversion_kind_to_c_str; };
        template<> struct Conversion_kind_t_<Conversion_kind_value::to_string>{ using T = Conversion_kind_to_string; };
        template<> struct Conversion_kind_t_<Conversion_kind_value::to_string_view>{ using T = Conversion_kind_to_string_view; };
        template<> struct Conversion_kind_t_<Conversion_kind_value::none>{ using T = Conversion_kind_none; };

        template< Conversion_kind_value kind >
        using Conversion_kind_ = typename Conversion_kind_t_<kind>::T;

        template< class T >
        inline auto as_string_append_argument( const T& value, Conversion_kind_general )
            -> string
        {
            ostringstream stream;
            stream << value;
            return stream.str();
        }

        template< class T >
        inline auto as_string_append_argument( const T& value, Conversion_kind_to_bool )
            -> string_view
        { return as_string_append_argument( bool( value ) ); }

        template< class T >
        inline auto as_string_append_argument( const T& value, Conversion_kind_to_c_str )
            -> C_str
        { return as_string_append_argument( C_str( value ) ); }

        template< class T >
        inline auto as_string_append_argument( const T& value, Conversion_kind_to_string )
            -> string
        { return as_string_append_argument( string( value ) ); }

        template< class T >
        inline auto as_string_append_argument( const T& value, Conversion_kind_to_string_view )
            -> string_view
        { return as_string_append_argument( string_view( value ) ); }

        template< class T >
        inline auto as_string_append_argument( const T& value, Conversion_kind_none )
            -> C_str
            = delete;

        template< class T >
        inline auto as_string_append_argument( const T& value )
            -> auto
        {  return as_string_append_argument( value, Conversion_kind_<conversion_kind_value_<T>()>() ); }
    }  // namespace impl

    template< class T >
    inline auto str( T const& value )
        -> string
    { return string( impl::as_string_append_argument( value ) ); }

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
namespace kickstart::core               { using namespace text_conversion; }
