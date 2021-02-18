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

#include <kickstart/system-specific/Console.interface.hpp>

#include <kickstart/core/language/Truth.hpp>
#include <kickstart/system-specific/windows/api/consoles.hpp>
#include <kickstart/system-specific/windows/api/files.hpp>          // CreateFile
#include <kickstart/system-specific/windows/api/text-encoding.hpp>

#include <assert.h>         // assert
#include <limits.h>         // INT_MAX

#include <queue>
#include <string>
#include <utility>

namespace kickstart::system_specific::_definitions {
    using namespace kickstart::language;                // Truth etc.
    using namespace kickstart::collection_util;         // begin_ptr_of
    using   std::queue,
            std::wstring,
            std::move;

    inline auto open_console_input()
        -> winapi::HANDLE
    {
        using namespace winapi;
        const DWORD flags = 0;      // They're ignored.
        const HANDLE h = CreateFileW(
            L"conin$", generic_read, file_share_read, nullptr, open_existing, flags, {}
            );
        hopefully( h != invalid_handle_value )
            or KS_FAIL( "Windows’ CreateFileW failed to open console for input." );
        return h;
    }

    inline auto open_console_output()
        -> winapi::HANDLE
    {
        using namespace winapi;
        const DWORD flags = 0;      // They're ignored.
        const HANDLE h = CreateFileW(
            L"conout$", generic_write, file_share_write, nullptr, open_existing, flags, {}
            );
        hopefully( h != invalid_handle_value )
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

    class Windows_console:
        public Console
    {
        friend auto Console::instance() -> Console&;
        constexpr static int ctrl_z = 26;

        struct Input_state
        {
            Truth           at_start_of_line    = true;
            queue<int>      bytes               = {};
        };

        Input_state         m_input_state;
        winapi::HANDLE      m_input_handle      = {};
        winapi::HANDLE      m_output_handle     = {};

    public:
        void write_bytes( const string_view& s ) override
        {
            const int n = int_size( s );
            const int n_written = write( m_output_handle, begin_ptr_of( s ), n );
            hopefully( n_written == n )
                or KS_FAIL( "Failed to write to console." );
        }

        auto read_byte() -> int override
        {
            while( m_input_state.bytes.empty() ) {
                const wint_t    code        = read_widechar( m_input_handle );
                const Truth     soft_eof    = (m_input_state.at_start_of_line and code == ctrl_z);

                if( code != WEOF ) {
                    m_input_state.at_start_of_line = false;
                }

                // For now ignoring UTF-16 surrogate pair, treating all input as UCS-2.
                // TODO: check if Windows can yield surrogate pair keyboard input, and possibly support.
                if( soft_eof or code == WEOF ) {
                    m_input_state.bytes.push( EOF );
                } else if( not is_surrogate( code ) ) {
                    char buffer[32];        // Max UTF-8 length is 4, this is more than enough.

                    const winapi::DWORD     flags           = 0;
                    const wchar_t           code_as_wchar   = code;
                    const int               n_bytes         = winapi::WideCharToMultiByte(
                        winapi::cp_utf8, flags, &code_as_wchar, 1, buffer, sizeof( buffer ), nullptr, nullptr
                    );

                    for( int i = 0; i < n_bytes; ++i ) {
                        m_input_state.bytes.push( buffer[i] );
                    }
                }
            }
            const int result = m_input_state.bytes.front();
            m_input_state.bytes.pop();
            return result;
        }

        Windows_console():
            m_input_handle( open_console_input() ),
            m_output_handle( open_console_output() )
        {}

        static auto instance()
            -> Windows_console&
        {
            static Windows_console the_instance;
            return the_instance;
        }
    };

    inline auto Console::instance()
        -> Console&
    { return Windows_console::instance(); }

}  // namespace kickstart::system_specific::_definitions
