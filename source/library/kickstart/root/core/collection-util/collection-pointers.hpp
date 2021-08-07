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

#include <kickstart/root/core/language/type-aliases.hpp>     // Size

namespace kickstart::collection_util::_definitions {
    using kickstart::language::Size;

    template< class Array >        // TODO: enable_if random access
    inline auto begin_ptr_of( Array& a )
        -> auto
    { return a.data(); }

    template< class Item, Size n >
    inline auto begin_ptr_of( Item (&a)[n] )
        -> auto
    { return &a[0]; }

    template< class Array >        // TODO: enable_if random access
    inline auto end_ptr_of( Array& a )
        -> auto
    { return a.data() + a.size(); }

    template< class Item, Size n >
    inline auto end_ptr_of( Item (&a)[n] )
        -> auto
    { return &a[0] + n; }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::begin_ptr_of,
        d::end_ptr_of;
    }  // namespace exported names
}  // namespace kickstart::collection_util::_definitions

namespace kickstart::collection_util    { using namespace _definitions::exported_names; }
