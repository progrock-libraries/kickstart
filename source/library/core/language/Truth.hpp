// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/assertion-headers/~assert-reasonable-compiler.hpp>

#if 0
/// `Truth` is a drop-in replacement for `bool` without implicit conversion from/to types other
/// than `bool`.

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


#include <cppx-core-language/syntax/declarations.hpp>           // CPPX_USE_STD
#include <cppx-core-language/tmp/Enable_if_.hpp>                // cppx::Enable_if_

#include <type_traits>      // std::is_same_v

namespace cppx
{
    CPPX_USE_STD( enable_if_t, is_same_v );

    /// \brief A drop-in replacement for `bool` without implicit conversion from/to types
    /// other than `bool`.
    ///
    /// Implicit conversions to and from `bool` type are a common cause of
    /// undesired overload resolution. `Truth` only converts implicitly to and from
    /// `bool`. It's mostly a drop-in replacement for `bool` because you can use `Truth`
    /// values directly as conditions in `if` and loop statements, and in boolean
    /// operator expressions.
    ///
    /// “Mostly”: as of C++17 `Truth`, like any other user-defined class type, can't be
    /// used as template value parameter type, but this may change with C++20.
    ///
    /// Unplanned positive effect: indexing of a `std::vector<Truth>` produces a reference
    /// to a `Truth` item, which you can take the address of, rather than the proxy object
    /// that you usually get with a `std::vector<bool>`. The proxy objects of the latter
    /// allows it to store just 1 bit per item, at a cost that includes marginally reduced
    /// efficiency and high inconvenience. That cost is avoided with `Truth` as item type.

    template< class T >
    static constexpr bool is_bool_ = std::is_same_v<T, bool>;

    template< class Truth_class >
    struct Truth_values_
    {
        static const Truth_class yes;
        static const Truth_class no;
    };

    class Truth:
        public Truth_values_<Truth>
    {
        bool        m_value;

    public:
        /// \brief Implicit conversion to `bool` (only).
        ///
        /// Implicit conversion to `bool` because that's needed for template args.
        /// Restricted via SFINAE because overload resolution, want predictability.
        template<
            class Result,
            class = Enable_if_<is_bool_<Result>>
        >
            constexpr operator Result() const noexcept { return m_value; }

        /// \brief Construction from `bool` (only).
        ///
        /// Construction SFINAE-restricted to `bool` argument.
        template<
            class Arg,
            class = Enable_if_<is_bool_<Arg>>
        >
            constexpr Truth( const Arg value ) noexcept: m_value( value ) {}
    };

    template< class Truth_class >
    const Truth_class Truth_values_<Truth_class>::yes   = true;

    template< class Truth_class >
    const Truth_class Truth_values_<Truth_class>::no    = false;

    constexpr inline auto is_true( const Truth value )
        -> Truth
    { return value; }

    constexpr inline auto is_false( const Truth value )
        -> Truth
    { return not value; }

    constexpr inline auto operator+( const Truth value )
        -> int
    { return 0 + !!value; }

    constexpr inline auto operator!=( const Truth lhs, const Truth rhs )
        -> bool
    { return !!lhs != !!rhs; }

    constexpr inline auto operator<=( const Truth lhs, const Truth rhs )
        -> bool
    { return !!lhs <= !!rhs; }

    constexpr inline auto operator<( const Truth lhs, const Truth rhs )
        -> bool
    { return !!lhs < !!rhs; }

    constexpr inline auto operator==( const Truth lhs, const Truth rhs )
        -> bool
    { return !!lhs == !!rhs; }

    constexpr inline auto operator>=( const Truth lhs, const Truth rhs )
        -> bool
    { return !!lhs >= !!rhs; }

    constexpr inline auto operator>( const Truth lhs, const Truth rhs )
        -> bool
    { return !!lhs > !!rhs; }

}  // namespace cppx
#endif
