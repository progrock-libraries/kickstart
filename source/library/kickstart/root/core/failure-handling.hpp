// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/assertion-headers/~assert-reasonable-compiler.hpp>

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

#include <kickstart/root/core/stdlib-extensions/failure-handling/standard-exceptions.hpp>
#include <kickstart/root/core/stdlib-extensions/type-traits.hpp>     // is_derived_and_base
#include <kickstart/root/core/language/types/Truth.hpp>
#include <kickstart/root/core/language/types/type-aliases.hpp>

#include <kickstart/c/assert.hpp>

#include <string>
#include <string_view>
#include <utility>          // std::enable_if_t

#define KS_FAIL_( X, s )                                                        \
        ::kickstart::failure_handling::fail_<X>(                                \
            kickstart::failure_handling::Funcname{ __func__ }, std::string( s ) \
            );

#define KS_FAIL( s ) \
    KS_FAIL_( std::runtime_error, s )

namespace kickstart::failure_handling::_definitions {
    using namespace kickstart::language;
    using namespace kickstart::type_traits;
    using   std::string,
            std::string_view,
            std::enable_if_t;

    // For named parameter:
    struct Funcname
    {
        string_view     value;
    };


    [[noreturn]] inline void unreachable() { assert( false ); throw ~42; }

    inline constexpr auto hopefully( const Truth condition )
        -> Truth
    { return condition; }

    class A_success_checker {};

    struct Is_false: A_success_checker
    {
        auto denotes_success( const Truth x ) const -> Truth { return not x; }
    };

    struct Is_true: A_success_checker
    {
        auto denotes_success( const Truth x ) const -> Truth { return x; }
    };

    struct Is_zero: A_success_checker
    {
        template< class Value >
        auto denotes_success( const Value x ) const -> Truth { return x == 0; }
    };

    struct Is_nonzero: A_success_checker
    {
        template< class Value >
        auto denotes_success( const Value x ) const -> Truth { return x != 0; }
    };

    template< 
        class Value,
        class Checker,
        class = enable_if_t< is_derived_and_base_<Checker, A_success_checker> >
        >
    inline auto operator>>( const Value& v, const Checker& checker )
        -> Truth
    { return checker.denotes_success( v ); }

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
    { return fail_<std_exception::runtime_error>( s ); }

    //[[noreturn]]
    inline auto fail( const Funcname& funcname, const string& s = "" )
        -> Truth
    { return fail_<std_exception::runtime_error>( funcname, s ); }

    struct End_of_file_exception:
        std_exception::runtime_error
    { using runtime_error::runtime_error; };

    class Clean_app_exit_exception
    {
        std_exception::runtime_error    m_message;

    public:
        auto what() const
            noexcept
            -> C_str
        { return m_message.what(); }

        Clean_app_exit_exception( const string& s ):
            m_message( s )
        {}
    };

    inline auto fail_app( const string& s )
        -> Truth
    { return fail_<Clean_app_exit_exception>( "!" + s ); }

    inline auto fail_app_and_display( const string& s )
        -> Truth
    { return fail_<Clean_app_exit_exception>( s ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Funcname,
        d::unreachable,
        d::A_success_checker, d::Is_false, d::Is_true, d::Is_zero, d::Is_nonzero,
        d::hopefully,
        d::fail_,
        d::fail,
        d::End_of_file_exception,
        d::Clean_app_exit_exception,
        d::fail_app,
        d::fail_app_and_display;

        using namespace std_exception;
    }  // namespace exported names
}  // namespace kickstart::failure_handling::_definitions

namespace kickstart::failure_handling   { using namespace _definitions::exported_names; }
