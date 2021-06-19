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

#include <kickstart/system-specific/open-c-file.interface.hpp>

#include <kickstart/core/failure-handling.hpp>
#include <kickstart/core/language/type-aliases.hpp>
#include <kickstart/system-specific/windows/text-encoding-conversion.hpp>

#include <kickstart/c/string.hpp>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::failure_handling;

    inline auto raw_u8open_c_file( const C_str utf8_path, const C_str mode )
        -> C_file
    {
        const wstring utf16_path = to_utf16( utf8_path );
        const wstring utf16_mode = wstring( mode, mode + strlen( mode ) );
        return ::_wfopen( utf16_path.c_str(), utf16_mode.c_str() );
    }

    inline auto raw_open_c_file( const fs::path& path, const C_str mode )
        -> C_file
    {
        const wstring utf16_mode = wstring( mode, mode + strlen( mode ) );
        return ::_wfopen( path.c_str(), utf16_mode.c_str() );
    }

}  // namespace kickstart::system_specific::_definitions
