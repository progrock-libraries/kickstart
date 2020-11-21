﻿// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once

// kickstart.iostreams.hpp - minimal convenience functionality for C++ learners.
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

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

namespace ks::stdstuff {
    using   std::cin, std::cout, std::cerr, std::clog, std::endl, std::flush;   // From <iostream>.
    using   std::boolalpha, std::hex, std::dec, std::oct;                       // From <iostream>
    using   std::fixed, std::scientific, std::hexfloat, std::defaultfloat;      // From <iostream>.
    using   std::left, std::right, std::internal;                               // From <iostream>.
    using   std::setw, std::setprecision, std::setfill, std::quoted;            // From <iomanip>.
    using   std::ifstream, std::ofstream;                                       // From <fstream>.
    using   std::istringstream, std::ostringstream;                             // From <sstream>.
}  // namespace ks::stdstuff

namespace ks {
    using namespace stdstuff;
}  // namespace ks

namespace kickstart = ks;