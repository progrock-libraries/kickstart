//TODO:

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

#include <kickstart/root/system-specific/console-adapted-io-functions.interface.hpp>

#include <kickstart/root/system-specific/windows/api/consoles.hpp>
#include <kickstart/root/system-specific/windows/Console.impl.hpp>

#include <kickstart/c/stdio.hpp>    // Windows-specific function `_fileno`.
#include <io.h>                     // Windows-specific header, `_get_osfhandle`.

namespace kickstart::system_specific::_definitions {

    inline auto is_console( const C_file f )
        -> Truth
    {
        const int num = _fileno( f );
        // `_isatty` can produce false positives e.g. for a serial port, so:
        const intptr_t handle = _get_osfhandle( num );
        winapi::DWORD mode;
        return !!winapi::GetConsoleMode( winapi::HANDLE( handle ), &mode );
    }

    inline auto raw_input_or_none_from_console( const C_file )
        -> optional<string>
    { return Windows_console::instance().input_or_none(); }

    inline void raw_output_to_console( const C_file, const string_view& s )
    {
        Windows_console::instance().write_bytes( s );
    }

}  // namespace kickstart::system_specific::_definitions
