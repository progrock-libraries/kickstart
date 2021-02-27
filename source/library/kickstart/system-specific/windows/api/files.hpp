// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/system-specific/windows/api/~header-boilerplate-stuff.hpp>

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

#include <kickstart/system-specific/windows/api/types.hpp>  // Supplies CloseHandle

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
        using   ::CreateFileW;

        const DWORD generic_read            = GENERIC_READ;
        const DWORD generic_write           = GENERIC_WRITE;
        const DWORD file_share_read         = FILE_SHARE_READ;
        const DWORD file_share_write        = FILE_SHARE_WRITE;
        const DWORD open_existing           = OPEN_EXISTING;
    #else
        using namespace kickstart::winapi;

        const DWORD generic_read            = 0x80000000;
        const DWORD generic_write           = 0x40000000;
        const DWORD file_share_read         = 1;
        const DWORD file_share_write        = 2;
        const DWORD open_existing           = 3;

        extern "C" auto __stdcall CreateFileW(
            C_wstr                  lpFileName,
            DWORD                   dwDesiredAccess,
            DWORD                   dwShareMode,
            void*                   lpSecurityAttributes,
            DWORD                   dwCreationDisposition,
            DWORD                   dwFlagsAndAttributes,
            HANDLE                  hTemplateFile
            ) -> HANDLE;
    #endif


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::generic_read, d::generic_write,
        d::file_share_read, d::file_share_write,
        d::open_existing,
        d::CreateFileW;
    }  // namespace exported names
}  // namespace kickstart::winapi::_definitions

namespace kickstart::winapi { using namespace _definitions::exported_names; }
