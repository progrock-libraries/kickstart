// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/core/language/assertion-headers/~assert-reasonable-compiler.hpp>

// Copyright (c) 2020 Alf P. Steinbach. MIT license, with license text:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software isfailed
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

// [noreturn] specs commented out because with them Visual C++ 2019 issues “warning
// C4702: unreachable code” sillywarnings on use of `hopefully( something ) or fail()`.

#include <kickstart/core/stdlib-extensions/standard-exceptions.hpp>
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/core/language/type-aliases.hpp>

#include <assert.h>

#include <string>
#include <string_view>

#define KS_FAIL_( X, s )                                                        \
        ::kickstart::failure_handling::fail_<X>(                                \
            kickstart::failure_handling::Funcname{ __func__ }, std::string( s ) \
            );

#define KS_FAIL( s ) \
    KS_FAIL_( std::runtime_error, s )

namespace kickstart::failure_handling::_definitions {
    using namespace kickstart::language;
    namespace std_x = kickstart::std_x;
    using   std::string,
            std::string_view;

    // For named parameter:
    struct Funcname
    {
        string_view     value;
    };

    [[noreturn]] inline void unreachable() { assert( false ); throw ~42; }

    inline constexpr auto hopefully( const Truth condition )
        -> Truth
    { return condition; }

    template< class X >
    //[[noreturn]]
    inline auto fail_( const string& s )
        -> Truth
    { throw X( s ); }

    template< class X >
    //[[noreturn]]
    inline auto fail_( const Funcname& funcname, const string& s = "" )
        -> Truth
    {
        auto message = string( funcname.value );
        if( s.length() > 0 ) {
            message += " - ";
            message += s;
        } else {
            message += " failed";
        }
        return fail_<X>( message );
    }

    //[[noreturn]]
    inline auto fail( const string& s )
        -> Truth
    { return fail_<std_x::runtime_error>( s ); }

    //[[noreturn]]
    inline auto fail( const Funcname& funcname, const string& s = "" )
        -> Truth
    { return fail_<std_x::runtime_error>( funcname, s ); }

    struct End_of_file_exception:
        std_x::runtime_error
    { using runtime_error::runtime_error; };

    class Clean_app_exit_exception:
        public std_x::Base
    {
        std_x::runtime_error    m_message;

    public:
        auto what() const
            noexcept
            -> C_str override
        { return m_message.what(); }

        Clean_app_exit_exception( const string& s ):
            m_message( s )
        {}
    };

    inline auto fail_app( const string& s )
        -> Truth
    { return fail_<Clean_app_exit_exception>( s ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Funcname,
        d::unreachable,
        d::hopefully,
        d::fail_,
        d::fail,
        d::End_of_file_exception,
        d::Clean_app_exit_exception,
        d::fail_app;

        using namespace std_x;
    }  // namespace exported names
}  // namespace kickstart::failure_handling::_definitions

namespace kickstart::failure_handling   { using namespace _definitions::exported_names; }
