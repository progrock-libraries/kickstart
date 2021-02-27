// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once

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

#ifndef KS_OS_IS_WIN64
#   error "This header is for 64-bit Windows systems only."
#   include <nosuch>
#endif
static_assert( sizeof( void* ) == 8 );  // 64-bit system
#include <kickstart/core/language/assertion-headers/~assert-reasonable-compiler.hpp>

#include <kickstart/core/language/type-aliases.hpp>

// Part of workaround for sabotage-like Visual C++ 2019 behavior for “extern "C"” funcs:
#if defined( KS_USE_WINDOWS_H ) || defined( BOOST_USE_WINDOWS_H )
#   include <windows.h>
#endif
