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

#include <limits.h>     // CHAR_BIT
#include <stddef.h>     // size_t, ptrdiff_t
#include <stdint.h>     // int32_t, int64_t

namespace kickstart::_definitions {
    using Size      = ptrdiff_t;
    using Index     = ptrdiff_t;
    using C_str     = const char*;
    using Byte      = unsigned char;

    using Int       = int64_t;
    using Float     = double;       static_assert( sizeof( double )*CHAR_BIT >= 64, "!" );

    using Int32     = int32_t;
    using Float32   = float;

    template< class T > using Type_ = T;


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Size, d::Index, d::C_str, d::Byte,
        d::Int, d::Float, d::Int32, d::Float32,
        d::Type_;
    }  // namespace exported names
}  // namespace kickstart::_definitions

namespace kickstart::type_aliases   { using namespace _definitions::exported_names; }
namespace kickstart::all            { using namespace type_aliases; }
