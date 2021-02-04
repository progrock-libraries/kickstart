// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../assertion-headers/~assert-reasonable-compiler.hpp"

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

#include "language/stdlib-extensions/standard-exceptions.hpp"
#include "language/type-aliases.hpp"

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
    namespace x = kickstart::language::x;
    using   std::string,
            std::string_view;

    struct Funcname
    {
        string_view     value;
    };

    [[noreturn]] inline void unreachable() { assert( false ); throw ~42; }

    inline auto hopefully( const bool condition )
        -> bool
    { return condition; }

    template< class X >
    [[noreturn]]
    inline auto fail_( const string& s )
        -> bool
    { throw X( s ); }

    template< class X >
    [[noreturn]]
    inline auto fail_( const Funcname& funcname, const string& s = "" )
        -> bool
    {
        auto message = string( funcname.value );
        if( s.length() > 0 ) {
            message += " - ";
            message += s;
        } else {
            message += " failed";
        }
        fail_<X>( message );
    }

    [[noreturn]]
    inline auto fail( const string& s )
        -> bool
    { fail_<x::runtime_error>( s ); }

    [[noreturn]]
    inline auto fail( const Funcname& funcname, const string& s = "" )
        -> bool
    { fail_<x::runtime_error>( funcname, s ); }

    class Clean_app_exit_exception:
        public x::Base
    {
        x::runtime_error    m_message;

    public:
        auto what() const
            noexcept
            -> C_str override
        { return m_message.what(); }

        Clean_app_exit_exception( const string& s ):
            m_message( s )
        {}
    };

    inline auto exit_app_with_message( const string& s )
        -> bool
    { return fail_<Clean_app_exit_exception>( s ); }

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        std::exception, std::logic_error, std::range_error, std::runtime_error,
        d::Funcname,
        d::unreachable,
        d::hopefully,
        d::fail_,
        d::fail,
        d::Clean_app_exit_exception,
        d::exit_app_with_message;
    }  // namespace exported names
}  // namespace kickstart::failure_handling::_definitions

namespace kickstart::failure_handling   { using namespace _definitions::exported_names; }
namespace kickstart::core               { using namespace failure_handling; }
