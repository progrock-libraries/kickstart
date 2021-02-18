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

#include <kickstart/core/collection-util.hpp>           // int_size
#include <kickstart/core/language/Truth.hpp>

#include <string>
#include <string_view>
#include <vector>

namespace kickstart::language::_definitions {
    using namespace std::string_view_literals;  // ""sv
    using   kickstart::language::Truth;
    using   std::string,
            std::string_view,
            std::vector;

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
        -> Truth
    { return s.substr( 0, substring.size() ) == substring; }

    inline auto starts_with( const char ch, const string_view& s )
        -> Truth
    { return s.size() >= 1 and s.front() == ch; }

    inline auto ends_with( const string_view& substring, const string_view& s )
        -> Truth
    {
        const size_t nss = substring.size();
        const size_t ns = s.size();
        return nss <= ns and s.substr( ns - nss ) == substring;
    }

    inline auto ends_with( const char ch, const string_view& s )
        -> Truth
    { return s.size() >= 1 and s.back() == ch; }

    template< class Func >
    inline auto for_each_part_of( const string_view& s, const string_view& delimiter, const Func& f )
    {
        const size_t    npos    = string_view::npos;
        const size_t    n       = s.size();

        size_t i_begin = 0;
        for( ;; ) {
            const size_t i_end = s.find( delimiter, i_begin );
            if( i_end == npos ) { break; }
            f( string_view( s.data() + i_begin, i_end - i_begin ) );
            i_begin = i_end + delimiter.size();
        }
        if( i_begin < n ) {     // This is the main case, most common code path.
            f( string_view( s.data() + i_begin, n - i_begin ) );
        }
    }

    inline auto split( const string_view& s, const string_view& delimiter )
        -> vector<string_view>
    {
        vector<string_view> result;
        for_each_part_of( s, delimiter, [&]( const auto& part ) { result.push_back( part ); } );
        return result;
    }

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::repeated_times, d::operator*,
        d::spaces,
        d::starts_with, d::ends_with,
        d::for_each_part_of, d::split;
    }  // namespace exported names
}  // namespace kickstart::language::_definitions

namespace kickstart::language       { using namespace _definitions::exported_names; }
namespace kickstart::core           { using namespace language; }
