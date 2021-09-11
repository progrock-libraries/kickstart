// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/main_library/system-specific/windows/api/~header-boilerplate-stuff.hpp>

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

#include <kickstart/c/stdint.hpp>   // int32_t, uint32_t, uintptr_t

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
        using   ::BOOL, ::DWORD, ::HANDLE, ::HMODULE, ::HWND, ::UINT;

        const int       max_path                = MAX_PATH;
        const HANDLE    invalid_handle_value    = INVALID_HANDLE_VALUE;

        using   ::CloseHandle;
    #else
        using BOOL      = int32_t;      // Or just `int`, which is 32-bit in Windows.
        using DWORD     = uint32_t;
        using HANDLE    = void*;
        using HWND      = struct Window_handle_t*;
        using HMODULE   = struct Module_handle_t*;
        using UINT      = uint32_t;

        const int       max_path                = 260;
        const HANDLE    invalid_handle_value    = HANDLE( uintptr_t( -1 ) );

        extern "C" auto __stdcall CloseHandle( HANDLE hObject ) -> BOOL;
    #endif


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::BOOL, d::DWORD, d::HANDLE, d::UINT,
        d::max_path, d::invalid_handle_value,
        d::CloseHandle;
    }  // namespace exported names
}  // namespace kickstart::winapi::_definitions

namespace kickstart::winapi     { using namespace _definitions::exported_names; }
