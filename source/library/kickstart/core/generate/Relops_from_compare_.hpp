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

#include <kickstart/core/language/Truth.hpp>

namespace kickstart::generate::_definitions {
    using kickstart::language::Truth;

    template< class Derived >
    struct Relops_from_compare_
    {
        friend
        inline constexpr auto operator<( const Derived& a, const Derived& b )
            -> Truth
        { return (compare( a, b ) < 0); }

        friend
        inline constexpr auto operator<=( const Derived& a, const Derived& b )
            -> Truth
        { return (compare( a, b ) <= 0); }

        friend
        inline constexpr auto operator==( const Derived& a, const Derived& b )
            -> Truth
        { return (compare( a, b ) == 0); }

        friend
        inline constexpr auto operator>=( const Derived& a, const Derived& b )
            -> Truth
        { return (compare( a, b ) >= 0); }

        friend
        inline constexpr auto operator>( const Derived& a, const Derived& b )
            -> Truth
        { return (compare( a, b ) > 0); }

        friend
        inline constexpr auto operator!=( const Derived& a, const Derived& b )
            -> Truth
        { return (compare( a, b ) != 0); }
    };

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Relops_from_compare_;
    }  // namespace exported names
}  // namespace kickstart::generate::_definitions

namespace kickstart::generate   { using namespace _definitions::exported_names; }
