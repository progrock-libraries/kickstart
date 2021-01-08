// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "winapi-header-boilerplate-stuff.hpp"

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

#include "../../system-api/windows/winapi-types.hpp"

#include <assert.h>     // assert
#include <stdint.h>     // uint32_t
#include <io.h>         // _get_osfhandle
#include <limits.h>     // INT_MAX
#include <wchar.h>      // wint_t, WEOF

#include <queue>        // std::queue
#include <string>       // std::wstring

namespace kickstart::winapi::_definitions {
    using namespace kickstart::language;        // Type_ etc.

    // Visual C++ 2019 (16.3.3) and later may issue errors on the Windows API function
    // declarations here when <windows.h> is also included, as explained in
    //
    // “Including Windows.h and Boost.Interprocess headers leads to C2116 and C2733”
    // https://developercommunity.visualstudio.com/content/problem/756694/including-windowsh-and-boostinterprocess-headers-l.html
    //
    // A fix is to use the per October 2019 undocumented option “/Zc:externC-”.
    //
    // A more fragile fix is to include <windows.h> BEFORE any Kickstart header, or
    // to define KS_USE_WINDOWS_H or BOOST_USE_WINDOWS_H or both in the build.

#ifdef MessageBox       // <windows.h> has been included
    using   ::GetConsoleMode, ::SetConsoleMode,
            ::ReadConsoleW, ::WriteConsoleW;

    const auto enable_virtual_terminal_processing   = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    const auto enable_extended_flags                = ENABLE_EXTENDED_FLAGS;
#else
    using namespace kickstart::winapi;
    const DWORD enable_virtual_terminal_processing  = 0x0004;
    const DWORD enable_extended_flags               = 0x0080;
    //const DWORD enable_virtual_terminal_input       = 0x0200;

    extern "C" auto __stdcall GetConsoleMode( HANDLE  hConsoleHandle, DWORD* lpMode )
        -> BOOL;
    extern "C" auto __stdcall SetConsoleMode( HANDLE  hConsoleHandle, DWORD dwMode )
        -> BOOL;

    extern "C" auto __stdcall ReadConsoleW(
        HANDLE                          hConsoleInput,
        void*                           lpBuffer,
        DWORD                           nNumberOfCharsToRead,
        DWORD*                          lpNumberOfCharsRead,
        void*                           pInputControl
        ) -> BOOL;

    extern "C" auto __stdcall WriteConsoleW(
        HANDLE          hConsoleOutput,
        const void*     lpBuffer,
        DWORD           nNumberOfCharsToWrite,
        DWORD*          lpNumberOfCharsWritten,
        void*           lpReserved
        ) -> BOOL;
#endif

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::enable_virtual_terminal_processing,
        d::enable_extended_flags,
        d::GetConsoleMode,
        d::SetConsoleMode,
        d::ReadConsoleW,
        d::WriteConsoleW;
    }  // namespace exported names
}  // namespace kickstart::utf8_io::standard_streams::_definitions

namespace kickstart::winapi { using namespace _definitions::exported_names; }
