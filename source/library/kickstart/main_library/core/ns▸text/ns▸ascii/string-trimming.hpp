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

#include <kickstart/main_library/core/ns▸collection_support.hpp>            // int_size
#include <kickstart/main_library/core/stdlib-extensions//ns▸strings.hpp>    // spaces
#include <kickstart/main_library/core/ns▸language/types/Truth.hpp>
#include <kickstart/main_library/core/text-encoding/ns▸ascii/character-util.hpp>

#include <string>
#include <string_view>

namespace kickstart::ascii::_definitions {
    using namespace kickstart::collection_support;  // begin_ptr_of, end_ptr_of, int_size
    using namespace kickstart::language;            // Truth
    using namespace kickstart::strings;             // spaces
    using namespace std::string_view_literals;      // ""sv
    using   std::string,
            std::string_view;

    namespace impl {
        constexpr unsigned remove_prefix = 1u << 0;
        constexpr unsigned remove_suffix = 1u << 1;

        template< unsigned flags >
        inline auto trimmed_( const string_view& s )
            -> string_view
        {
            const char* p_first     = begin_ptr_of( s );
            const char* p_beyond    = end_ptr_of( s );

            if constexpr( !!(flags & remove_prefix) ) {
                while( p_first != p_beyond and is( ascii::whitespace, *p_first ) ) {
                    ++p_first;
                }
            }
            if constexpr( !!(flags & remove_suffix) ) {
                while( p_beyond != p_first and is( ascii::whitespace, *(p_beyond - 1) ) ) {
                    --p_beyond;
                }
            }
            return {p_first, static_cast<size_t>( p_beyond - p_first )};
        }
    }  // namespace impl

    inline auto prefixtrimmed_view( const string_view& s )
        -> string_view
    { return impl::trimmed_<impl::remove_prefix>( s ); }

    inline auto suffixtrimmed_view( const string_view& s )
        -> string_view
    { return impl::trimmed_<impl::remove_suffix>( s ); }

    inline auto prefixtrimmed_string( const string_view& s )
        -> string
    { return string( prefixtrimmed_view( s ) ); }

    inline auto suffixtrimmed_string( const string_view& s )
        -> string
    { return string( suffixtrimmed_view( s ) ); }

    inline auto trimmed_view( const string_view& s )
        -> string_view
    { return impl::trimmed_<impl::remove_prefix | impl::remove_suffix>( s ); }

    inline auto trimmed_string( const string_view& s )
        -> string
    { return string( trimmed_view( s ) ); }

    inline auto trimmed( const string_view& s )
        -> string_view
    { return trimmed_view( s ); }

    inline auto trimmed( const string& s )
        -> string
    { return trimmed_string( s ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::prefixtrimmed_view, d::suffixtrimmed_view,
        d::prefixtrimmed_string, d::suffixtrimmed_string,
        d::trimmed_view, d::trimmed_string,
        d::trimmed;
    }  // namespace exported names
}  // namespace kickstart::ascii::_definitions

namespace kickstart::ascii  { using namespace _definitions::exported_names; }
