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

#include <stddef.h>     // size_t, ptrdiff_t

#include <array>
#include <functional>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace kickstart::stdlib {
    using namespace std::literals;      // E.g. being able to write `"hello"s` and `42s`.

    // Collections.
    using   std::array;                                                 // From <array>.
    using   std::begin, std::end, std::size;                            // From <iterator>.
    #ifdef __cpp_lib_ssize
        using std::ssize;
    #else
        template< class T >
        auto ssize( const T& c ) -> ptrdiff_t { return static_cast<ptrdiff_t>( size( c ) ); }
    #endif
    using   std::string;                                                // From <string>.
    using   std::string_view;                                           // From <string_view>.
    using   std::vector;                                                // From <vector>.


    // Misc.
    using   std::function;                                              // From <functional>.
    using   std::optional;                                              // From <optional>.
    using   std::exchange, std::forward, std::move, std::pair;          // From <utility>.
}  // namespace kickstart::stdlib

namespace kickstart::core   { using namespace stdlib; }
