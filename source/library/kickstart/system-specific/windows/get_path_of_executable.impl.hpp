// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#ifndef _WIN64
#   error "This header is for 64-bit Windows systems only."
#   include <nosuch>
#endif
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

#include <kickstart/system-specific/get_path_of_executable.interface.hpp>

#include <kickstart/core/collection-util/collection-sizes.hpp>              // int_size
#include <kickstart/core/failure-handling.hpp>
#include <kickstart/core/text-conversion/to-text/string-output-operator.hpp>
#include <kickstart/system-specific/windows/api/error-handling.hpp>         // GetLastError, SetLasError
#include <kickstart/system-specific/windows/api/process-info.hpp>           // GetModuleFileName
#include <kickstart/system-specific/windows/text-encoding-conversion.hpp>   // to_utf8

#include <string>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::failure_handling;
    using namespace kickstart::text_conversion;     // operator<<
    using namespace kickstart::collection_util;
    namespace winapi = kickstart::winapi;

    using   std::string, std::wstring;

    inline auto get_path_of_executable()
        -> string
    {
        auto path = wstring( winapi::max_path, '\0' );
        winapi::SetLastError( 0 );
        const int n_chars = winapi::GetModuleFileNameW( {}, path.data(), int_size( path ) );
        const unsigned error_code = winapi::GetLastError();
        hopefully( error_code == 0 )
            or KS_FAIL( ""s << "Winapi GetModuleFileName failed with error code " << error_code << "." );
        path.resize( n_chars );
        return to_utf8( path );
    }
}  // namespace kickstart::system_specific::_definitions
