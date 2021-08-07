// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/root/system-specific/windows/api/~header-boilerplate-stuff.hpp>

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

#include <kickstart/root/system-specific/windows/api/types.hpp>

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
        using   ::GetACP, ::MultiByteToWideChar, ::WideCharToMultiByte;
        const auto cp_utf8 = CP_UTF8;
        const auto mb_err_invalid_chars = MB_ERR_INVALID_CHARS;
    #else
        using namespace kickstart::winapi;
        const UINT cp_utf8                  = 65001;
        const DWORD mb_err_invalid_chars    = 8;

        extern "C" auto __stdcall GetACP()
            -> UINT;

        extern "C" auto __stdcall MultiByteToWideChar(
            UINT                            CodePage,
            DWORD                           dwFlags,
            const char*                     lpMultiByteStr,
            int                             cbMultiByte,
            wchar_t*                        lpWideCharStr,
            int                             cchWideChar
            ) -> int;

        extern "C" auto __stdcall WideCharToMultiByte(
            UINT                            CodePage,
            DWORD                           dwFlags,
            const wchar_t*                  lpWideCharStr,
            int                             cchWideChar,
            char*                           lpMultiByteStr,
            int                             cbMultiByte,
            const char*                     lpDefaultChar,
            BOOL*                           lpUsedDefaultChar
            ) -> int;
    #endif

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::cp_utf8, d::mb_err_invalid_chars,
        d::GetACP,
        d::MultiByteToWideChar,
        d::WideCharToMultiByte;
    }  // namespace exported names
}  // namespace kickstart::utf8_io::standard_streams::_definitions

namespace kickstart::winapi { using namespace _definitions::exported_names; }
