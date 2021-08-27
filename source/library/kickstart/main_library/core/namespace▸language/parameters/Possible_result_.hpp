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

#include <kickstart/main_library/core/namespace▸language/syntax/Has_value_.hpp>

#include <assert.h>

#include <exception>        // std::(exception_ptr, rethrow_exception)
#include <optional>         // std::optional
#include <utility>          // std::move, std::swap

namespace kickstart::language::_definitions {
    using   std::current_exception, std::exception_ptr, std::rethrow_exception, // <exception>
            std::optional,                          // <optional>
            std::exception,                         // <stdexcept>
            std::exchange, std::move, std::swap;    // <utility>

    template< class Tp_value >
    class Possible_result_
    {
    public:
        using Value = Tp_value;

    private:
        optional<Value>     m_opt_value;
        exception_ptr       m_exception;

        auto class_invariant() const
            -> bool
        { return not( m_opt_value and m_exception ); }

        void if_no_value_attempt_specific_throw() const
        {
            if( m_exception ) { rethrow_exception( m_exception ); }
        }

    public:
        Possible_result_()
        {
            assert( class_invariant() );
        }

        Possible_result_( Value v ):
            m_opt_value( move( v ) ), m_exception()
        { assert( class_invariant() ); }

        Possible_result_( exception_ptr e ):
            m_opt_value(), m_exception( move( e ) )
        { assert( class_invariant() ); }

        Possible_result_( const exception& x ):
            Possible_result_()
        {
            try{ throw x; } catch( ... ) { m_exception = current_exception(); }
            assert( class_invariant() );
        }

        Possible_result_(const Possible_result_& other):
            m_opt_value( other.m_opt_value ), m_exception( other.m_exception )
        {
            assert( class_invariant() );
        }

        Possible_result_( Possible_result_&& other ):
            m_opt_value( move( other.m_opt_value ) ), m_exception( move( other.m_exception ) )
        {
            assert( class_invariant() );
            assert( other.class_invariant() );
        }

        friend void swap( Possible_result_& a, Possible_result_& b ) noexcept
        {
            swap( a.m_opt_value, b.m_opt_value );
            swap( a.m_exception, b.m_exception );
        }

        auto operator=( const Possible_result_& other )
            -> Possible_result_&
        {
            Possible_result_ temp = other;
            swap( *this, temp );
            return *this;
        }

        auto operator=( Possible_result_&& other )
            -> Possible_result_&
        {
            swap( *this, other );
            return *this;
        }

        auto has_value() const
            -> bool
        { return m_opt_value.has_value(); }

        auto value() const
            -> Value
        {
            if_no_value_attempt_specific_throw();
            return m_opt_value.value();         // Throws if `m_opt_value` is empty.
        }

        auto value_or( const Value& a_default ) const noexcept
            -> Value
        { return (has_value()? m_opt_value.value() : a_default); }

        auto moved_value()
            -> Value
        {
            if_no_value_attempt_specific_throw();
            optional<Value> result = {};
            swap( result, m_opt_value );
            return move( result.value() );  // Throws if `result` is empty.
        }

        auto moved_value_or( Value a_default )
            -> Value
        { return move( has_value()? moved_value() : a_default ); }

        operator Value() const &    { return value(); }
        operator Value() const &&   { return value(); }
        operator Value() &&         { return moved_value(); }

        auto operator+() const &    -> Value    { return value(); }
        auto operator+() const &&   -> Value    { return value(); }
        auto operator+() &&         -> Value    { return moved_value(); }
    };

    template< class Value >
    using Result_has_value_ = Has_value_<Possible_result_<Value>>;

    namespace d = _definitions;
    namespace exported_names { using
        d::Possible_result_,
        d::Result_has_value_;
    }  // namespace exported names
}  // namespace kickstart::language::_definitions

namespace kickstart::language    { using namespace _definitions::exported_names; }
