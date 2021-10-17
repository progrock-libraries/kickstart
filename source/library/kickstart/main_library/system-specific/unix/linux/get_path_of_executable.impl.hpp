// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/assertion-headers/~assert-reasonable-compiler.hpp>
#ifndef KS_OS_IS_LINUX
#   error "This header is for Linux systems only."
#   include <nosuch>
#endif

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

#include <kickstart/main_library/core/failure-handling.hpp>
#include <kickstart/main_library/core/ns▸text_conversion/to-text/string-output-operator.hpp>

#include <unistd.h>     // pathconf, readlink
#include <kickstart/c/assert.hpp>
#include <kickstart/c/string.hpp>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::failure_handling;    // hopefully
    using namespace kickstart::text_conversion;     // ""s, operator<< for strings.

    using std::string;

    // TODO: move to header of reusable support stuff.
    inline auto general_maxpath()
        -> long
    {
        static const int the_value = ::pathconf( "/", _PC_PATH_MAX );
        #ifdef PATH_MAX
            return (the_value? the_value : PATH_MAX);
        #else
            assert( the_value > 0 );
            return the_value;
        #endif
    }

    // This should also work in Solaris, not just in Linux, but that's not tested.
    inline auto get_path_of_executable()
        -> string
    {
        const auto& procpath = "/proc/self/exe";    // “self” provides getpid()
        auto result = string( general_maxpath(), '\0' );
        const int n_bytes = ::readlink( procpath, result.data(), result.size() );
        hopefully( n_bytes >= 0 )
            or KS_FAIL( ""s << "::readlink failed to read “" << procpath << "”." )
        hopefully( n_bytes != 0 )
            or KS_FAIL( ""s << "::readlink obtained 0 bytes." );
        result.resize( n_bytes );
        return result;
    }
}  // namespace kickstart::system_specific::_definitions
