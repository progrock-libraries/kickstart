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

#include "type_aliases.hpp"

#include <stdexcept>
#include <string>

#define KS_FAIL_( X, s )                                        \
    ::kickstart::failure_handling::fail_<X>(                    \
        std::string() + __func__ + " - " + std::string( s )     \
        )
#define KS_FAIL( s ) \
    KS_FAIL_( std::runtime_error, s )

namespace kickstart::failure_handling::_definitions {
    using namespace kickstart::type_aliases;
    using   std::exception, std::runtime_error,
            std::string;

    inline auto hopefully( const bool condition ) -> bool { return condition; }

    template< class X >
    inline auto fail_( const string& s ) -> bool { throw X( s ); }

    inline auto fail( const string& s ) -> bool { return fail_<runtime_error>( s ); }

    class Clean_app_exit_exception:
        public exception
    {
        runtime_error   m_message;

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
        std::exception, std::logic_error, std::runtime_error,
        d::hopefully,
        d::fail_,
        d::fail,
        d::Clean_app_exit_exception,
        d::exit_app_with_message;
    }  // namespace exported names
}  // namespace kickstart::failure_handling::_definitions

namespace kickstart::failure_handling   { using namespace _definitions::exported_names; }
namespace kickstart::all                { using namespace failure_handling; }
