﻿// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "~assert-is-c++17-or-later.hpp"

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

#if defined( __clang__ )
# // Need to exclude clang explicitly because it pretends to be both Visual C++ and g++.
#elif defined( _MSC_VER ) && !defined( KS_ASSUME_BOOLEAN_OPERATOR_NAMES_PLEASE )
#   if _MSC_VER < 1911  // Visual Studio 2017 version 15.3, assumed "/permissive-" intro.
#       ifndef and
            static_assert( false,
                "Requires standard boolean keywords like `and`. For MSVC use e.g."
                " `/std:c++17 /Zc:__cplusplus /FI\"iso646.h\" /EHsc /wd4459 /utf-8`."
                );
#           include <requires_standard_boolean_keywords>
#       endif
#   endif
#endif
