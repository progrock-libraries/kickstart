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

#include <kickstart/main_library/core/ns▸collection_utility.hpp>            // int_size
#include <kickstart/main_library/core/stdlib-extensions//ns▸strings.hpp>    // spaces
#include <kickstart/main_library/core/ns▸language/types/Truth.hpp>
#include <kickstart/main_library/core/text-encoding/ns▸ascii/character-util.hpp>

#include <string>
#include <string_view>

namespace kickstart::ascii::_definitions {
    using namespace kickstart::collection_utility;     // begin_ptr_of, end_ptr_of, int_size
    using namespace kickstart::language;            // Truth
    using namespace kickstart::strings;             // spaces
    using namespace std::string_view_literals;      // ""sv
    using   std::string,
            std::string_view;

    inline auto left( const int minimum_new_size, const string_view& s )
        -> string
    { return string( s ) + spaces( minimum_new_size - int_size( s ) ); }

    inline auto right( const int minimum_new_size, const string_view& s )
        -> string
    { return spaces( minimum_new_size - int_size( s ) ) + string( s ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::left,
        d::right;
    }  // namespace exported names
}  // namespace kickstart::ascii::_definitions

namespace kickstart::ascii  { using namespace _definitions::exported_names; }
