//TODO:

// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#ifndef _WIN64
#   error "This header is for 64-bit Windows systems only."
#   include <nosuch>
#endif
#include <kickstart/core/language/assertion-headers/~assert-reasonable-compiler.hpp>

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

#include <kickstart/system-specific/console-io-functions.interface.hpp>

#include <kickstart/core/collection-util/collection-pointers.hpp>       // begin_ptr_of
#include <kickstart/core/failure-handling.hpp>
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/system-specific/windows/api/consoles.hpp>
#include <kickstart/system-specific/windows/api/files.hpp>              // CreateFile
#include <kickstart/system-specific/windows/api/text-encoding.hpp>

#include <stdio.h>

#include <string>
#include <string_view>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::failure_handling;
    using namespace kickstart::collection_util;
    using namespace kickstart::language;            // Size, Index
    using   std::string, std::wstring,
            std::string_view;

    using C_file = FILE*;

    inline auto open_console_input()
        -> winapi::HANDLE
    {
        const winapi::DWORD flags = 0;      // They're ignored.
        const winapi::HANDLE h = winapi::CreateFileW(
            L"conin$", winapi::generic_read, winapi::file_share_read, nullptr, winapi::open_existing, flags, {}
        );
        hopefully( h != winapi::invalid_handle_value )
            or KS_FAIL( "Windows’ CreateFileW failed to open console for input." );
        return h;
        winapi::    }

    inline auto open_console_output()
        -> winapi::HANDLE
    {
        const winapi::DWORD flags = 0;      // They're ignored.
        const winapi::HANDLE h = winapi::CreateFileW(
            L"conout$", winapi::generic_write, winapi::file_share_write, nullptr, winapi::open_existing, flags, {}
        );
        hopefully( h != winapi::invalid_handle_value )
            or KS_FAIL( "Windows’ CreateFileW failed to open console for output." );
        return h;
    }

    inline auto read_widechar( const winapi::HANDLE h )
        -> wint_t
    {
        for( const int dummy : {1, 2} ) {
            (void) dummy;
            wchar_t ch = 0;
            winapi::DWORD n_chars_read = 0;
            winapi::ReadConsoleW( h, &ch, 1, &n_chars_read, nullptr );
            if( n_chars_read == 0 ) {
                return WEOF;
            } else if( ch == L'\n' ) {
                continue;
            } else if( ch == L'\r' ) {
                return L'\n';
            }
            return ch;
        }
        return WEOF;
    }

    inline auto write( const winapi::HANDLE h, const Type_<const char*> buffer, const int n )
        -> int
    {
        assert( n > 0 );
        assert( n <= INT_MAX );

        auto ws = wstring( n, L'\0' );
        const int flags = 0;
        const int ws_len = winapi::MultiByteToWideChar(
            winapi::cp_utf8, flags, buffer, n, ws.data(), n
        );
        assert( ws_len > 0 );
        winapi::DWORD n_chars_written;
        winapi::WriteConsoleW( h, ws.data(), ws_len, &n_chars_written, nullptr );

        // Reporting the actual count of UTF-8 bytes written is costly, so fake it:
        return (int( n_chars_written ) < ws_len? 0 : n);
    }

    inline auto is_surrogate( const wchar_t )
        -> Truth
    { return false; }       // TODO:

    inline auto is_console( const C_file f ) -> bool;
    inline void raw_output_to_console( const C_file, const string_view& );
    inline auto raw_input_from_console( const C_file ) -> string;

}  // namespace kickstart::system_specific::_definitions
