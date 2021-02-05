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

#include <kickstart/core/language/collection-util.hpp>      // int_size

#include <string>
#include <string_view>

namespace kickstart::language::_definitions {
    using namespace std::string_view_literals;  // ""sv
    using   std::string,
            std::string_view;

    inline auto repeated_times( const int n, const string_view& s )
        -> string
    {
        string result;
        for( int i = 1; i <= n; ++i ) {
            result += s;
        }
        return result;
    }

    inline auto operator*( const int n, const string_view& s )
    -> string
    { return repeated_times( n, s ); }

    inline auto spaces( const int n )
        -> string
    { return n * " "sv; }

    inline auto starts_with( const string_view& substring, const string_view& s )
        -> bool
    { return s.substr( 0, substring.size() ) == substring; }

    inline auto starts_with( const char ch, const string_view& s )
        -> bool
    { return s.size() >= 1 and s.front() == ch; }

    inline auto ends_with( const string_view& substring, const string_view& s )
        -> bool
    {
        const size_t nss = substring.size();
        const size_t ns = s.size();
        return nss <= ns and s.substr( ns - nss ) == substring;
    }

    inline auto ends_with( const char ch, const string_view& s )
        -> bool
    { return s.size() >= 1 and s.back() == ch; }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::repeated_times, d::operator*,
        d::spaces,
        d::starts_with, d::ends_with;
    }  // namespace exported names
}  // namespace kickstart::language::_definitions

namespace kickstart::language       { using namespace _definitions::exported_names; }
namespace kickstart::core           { using namespace language; }
