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

#include <iostream>
#include <istream>          // Was not formally included by <iostream> prior to C++11.
#include <ostream>          // Was not formally included by <iostream> prior to C++11.

// Note:
//
// Iostreams i/o of ASCII text is portable by default, but while UTF-8 works in Unix it
// won't work by default in Windows.
//
// In particular, as of early 2021, with a Windows console window set to UTF-8 codepage
// 65001, stream input of non-ASCII characters just produces nullbytes at the Windows
// API level. And this is so also with Microsoft Terminal.
//
namespace kickstart::stdlib::iostreams {
    using   std::cin, std::cout, std::cerr, std::clog,      // From <iostream>.
            std::istream, std::ws,                          // From <istream>.
            std::ostream, std::endl;                        // From <ostream>.
}  // namespace kickstart::stdlib::iostreams
