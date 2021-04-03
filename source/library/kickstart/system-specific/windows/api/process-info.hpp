﻿// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
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

#include <kickstart/system-specific/windows/api/types.hpp>
#include "memory.hpp"           // LocalFree

#ifdef MessageBox           // <windows.h> has been included
#   include <shellapi.h>    // CommandLineToArgvW
#endif
#ifdef _MSC_VER
#     pragma comment( lib, "shell32.lib" )        // g++ links this implicitly.
#endif

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
        using   ::GetCommandLineW,
                ::CommandLineToArgvW,
                ::GetModuleFileNameW;
    #else
        using namespace kickstart::winapi;

        extern "C" auto __stdcall GetCommandLineW()
            -> wchar_t*;

        extern "C" auto __stdcall CommandLineToArgvW(
            const wchar_t*      lpCmdLine,
            int*                pNumArgs
            ) -> wchar_t**;

        extern "C" auto __stdcall GetModuleFileNameW(
            HMODULE     hModule,
            wchar_t*    lpFilename,
            DWORD       nSize
            ) -> DWORD;
#endif

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::GetCommandLineW,
        d::CommandLineToArgvW,
        d::GetModuleFileNameW;
    }  // namespace exported names
}  // namespace kickstart::winapi::_definitions

namespace kickstart::winapi { using namespace _definitions::exported_names; }
