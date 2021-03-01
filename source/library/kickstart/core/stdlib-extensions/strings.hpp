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

#include <kickstart/core/collection-util.hpp>                       // ssize, tail_of
#include <kickstart/core/language/Truth.hpp>                        // Truth
#include <kickstart/core/language/type-aliases.hpp>                 // C_str
#include <kickstart/core/text-encoding/ascii/character-util.hpp>    // ascii::whitespace

#include <initializer_list>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

namespace kickstart::strings::_definitions {
    using namespace std::string_view_literals;      // ""sv
    using namespace kickstart::collection_util;     // tail_of, ssize
    using namespace kickstart::language;            // Truth, C_str
    using   std::initializer_list,
            std::begin, std::end,
            std::string,
            std::string_view,
            std::vector;

    class C_str_ref
    {
        C_str       m_s;
        string      m_data;

    public:
        C_str_ref( const C_str s ): m_s( s ) {}
        C_str_ref( const string_view& s ): m_data( string( s ) ) { m_s = m_data.c_str(); }
        C_str_ref( const string& s ): m_s( s.c_str() ) {}

        auto s() const -> C_str { return m_s; }
        operator C_str () const { return m_s; }
    };

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

    inline auto split_on( const string_view& delimiter, const string_view& s )
        -> vector<string_view>
    {
        vector<string_view> result;
        for_each_part_of( s, delimiter, [&]( const auto& part ) { result.push_back( part ); } );
        return result;
    }

    inline auto split_on_whitespace( const string_view& s )
        -> vector<string_view>
    {
        vector<string_view> result;
        const Size n = ssize( s );
        Size i_begin = 0;
        Size i_end = 0;
        for( ;; ) {
            while( i_begin < n and is( ascii::whitespace, s[i_begin] ) ) {
                ++i_begin;
            }
            if( i_begin == n ) { break; }
            i_end = i_begin + 1;
            while( i_end < n and not is( ascii::whitespace, s[i_end] ) ) {
                ++i_end;
            }
            result.push_back( s.substr( i_begin, i_end - i_begin ) );
            i_begin = i_end;
        }
        return result;
    }

    template< class Iterator >
    inline auto joined_on(
        const string_view&      delimiter,
        const Iterator          it_begin,
        const Iterator          it_end
        ) -> string
    {
        if( it_begin == it_end ) {
            return "";
        }
        auto result = string( *it_begin );
        for( const string_view& s: tail_of( it_begin, it_end ) ) {
            result += delimiter;
            result += s;
        }
        return result;
    }

    inline auto joined_on(
        const string_view&              delimiter,
        initializer_list<string_view>&  parts
        ) -> string
    { return joined_on( delimiter, begin( parts ), end( parts ) ); }

    template< class Iterator >
    inline auto joined(
        const Iterator          it_begin,
        const Iterator          it_end
        ) -> string
    {
        if( it_begin == it_end ) {
            return "";
        }
        auto result = string();
        for( const string_view& s: all_of( it_begin, it_end ) ) {
            result += s;
        }
        return result;
    }

    inline auto joined( initializer_list<string_view>&  parts )
        -> string
    { return joined( begin( parts ), end( parts ) ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::C_str_ref,
        d::repeated_times, d::operator*,
        d::spaces,
        d::starts_with, d::ends_with,
        d::for_each_part_of,
        d::split_on,
        d::split_on_whitespace,
        d::joined_on, d::joined;
    }  // namespace exported names
}  // namespace kickstart::strings::_definitions

namespace kickstart::strings    { using namespace _definitions::exported_names; }
