// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../../system-specific/windows/api/~header-boilerplate-stuff.hpp"

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

#include "Interface.hpp"
#include "../../../core/language/type-aliases.hpp"
#include "../../../system-specific/windows/api/consoles.hpp"
#include "../../../system-specific/windows/api/text-encoding.hpp"

#include <assert.h>     // assert
#include <stdint.h>     // uint32_t
#include <io.h>         // _get_osfhandle
#include <limits.h>     // INT_MAX
#include <wchar.h>      // wint_t, WEOF

#include <queue>        // std::queue
#include <string>       // std::wstring

namespace kickstart::utf8_io::standard_streams::_definitions {
    using namespace kickstart::language;        // Type_ etc.

    using   std::queue,
            std::wstring;

    class For_windows
    {
        // Not copyable or movable.
        For_windows( const For_windows& ) = delete;
        auto operator=( const For_windows& ) -> For_windows& = delete;

        struct Console_data
        {
            FILE*           streams[3]      = {};
            winapi::HANDLE  input_handle    = {};
            winapi::HANDLE  output_handle   = {};
            winapi::DWORD   original_mode   = winapi::DWORD( -1 );
        };

        struct Input_state
        {
            bool            at_start_of_line    = true;
            queue<int>      bytes               = {};
        };

        class Console_io
        {
            constexpr static int ctrl_z = 26;

            static auto is_surrogate( const wchar_t ) //code_point )
                -> bool
            { return false; }       // TODO:

            static inline auto read_widechar( const winapi::HANDLE handle ) -> wint_t;

        public:
            static inline auto read_byte( const Type_<FILE*> ) -> int;
            static inline auto write( const Type_<const void*> buffer, const Size n, FILE* ) -> Size;
        };

        struct C_streams_io
        {
            static auto read_byte( const Type_<FILE*> f )
                -> int
            { return ::fgetc( f ); }

            static auto write( const Type_<const void*> buffer, const Size buffer_size, const Type_<FILE*> f)
                -> Size
            { return ::fwrite( buffer, 1, buffer_size, f ); }
        };

        Console_data    m_console;
        Input_state     m_input;

        ~For_windows()
        {
            if( m_console.original_mode != winapi::DWORD( -1 ) ) {
                winapi::SetConsoleMode( m_console.output_handle, m_console.original_mode );
            }
        }

        For_windows()
        {
            const Type_<FILE*> c_streams[] = {stdin, stdout, stderr};

            for( const int stream_id: {0, 1, 2} ) {
                const auto handle = reinterpret_cast<winapi::HANDLE>( _get_osfhandle( stream_id ) );
                winapi::DWORD dummy;
                const bool is_console = !!winapi::GetConsoleMode( handle, &dummy );
                if( is_console ) {
                    m_console.streams[stream_id] = c_streams[stream_id];
                    (stream_id == 0? m_console.input_handle : m_console.output_handle) = handle;
                }
            }

            if( m_console.output_handle ) {
                if( winapi::GetConsoleMode( m_console.output_handle, &m_console.original_mode ) ) {
                    const auto support_escapes =
                        (m_console.original_mode | winapi::enable_virtual_terminal_processing)
                        & ~winapi::enable_extended_flags;
                    winapi::SetConsoleMode( m_console.output_handle, support_escapes );
                }
            }
        }

    public:
        using Func = utf8_io::standard_streams::Interface::Func;

        auto read_byte_func_for( const Type_<FILE*> f ) const
            -> Func::Read_byte&
        {
            if( m_console.streams[0] == f ) {
                return *Console_io::read_byte;
            }
            return *C_streams_io::read_byte;
        }

        auto write_func_for( const Type_<FILE*> f ) const
            -> Func::Write&
        {
            for( const int i: {1, 2} ) {
                if( m_console.streams[i] == f ) {
                    return *Console_io::write;
                }
            }
            return *C_streams_io::write;
        }

        static auto singleton()
            -> For_windows&
        {
            static For_windows the_instance;
            return the_instance;
        }
    };

    inline auto For_windows::Console_io::read_widechar( const winapi::HANDLE handle )
        -> wint_t
    {
        for( const int dummy : {1, 2} ) {
            (void) dummy;
            wchar_t ch = 0;
            winapi::DWORD n_chars_read = 0;
            winapi::ReadConsoleW( handle, &ch, 1, &n_chars_read, nullptr );
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

    inline auto For_windows::Console_io::read_byte( const Type_<FILE*> )
        -> int
    {
        auto& streams = singleton();
        auto& input = streams.m_input;
        while( input.bytes.empty() ) {
            const wint_t    code        = read_widechar( streams.m_console.input_handle );
            const bool      soft_eof    = (input.at_start_of_line and code == ctrl_z);

            if( code != WEOF ) {
                input.at_start_of_line = false;
            }

            // For now ignoring UTF-16 surrogate pair, treating all input as UCS-2.
            // TODO: check if Windows can yield surrogate pair keyboard input, and possibly support.
            if( soft_eof or code == WEOF ) {
                input.bytes.push( EOF );
            } else if( not is_surrogate( code ) ) {
                char buffer[32];        // Max UTF-8 length is 4, this is more than enough.

                const winapi::DWORD     flags           = 0;
                const wchar_t           code_as_wchar   = code;
                const int               n_bytes         = winapi::WideCharToMultiByte(
                    winapi::cp_utf8, flags, &code_as_wchar, 1, buffer, sizeof( buffer ), nullptr, nullptr
                    );

                for( int i = 0; i < n_bytes; ++i ) {
                    input.bytes.push( buffer[i] );
                }
            }
        }

        const int result = input.bytes.front();
        input.bytes.pop();
        return result;
    }

    inline auto For_windows::Console_io::write( const Type_<const void*> buffer, const Size n, FILE* )
        -> Size
    {
        if( n <= 0 ) {
            return 0;
        }
        assert( n <= Size( INT_MAX ) );

        auto ws = wstring( n, L'\0' );
        const int flags = 0;
        const int ws_len = winapi::MultiByteToWideChar(
            winapi::cp_utf8, flags, static_cast<const char*>( buffer ), int( n ), ws.data(), int( n )
            );
        assert( ws_len > 0 );       // More precisely, the number of UTF-8 encoded code points.
        const auto handle = For_windows::singleton().m_console.output_handle;
        winapi::DWORD n_chars_written;
        winapi::WriteConsoleW( handle, ws.data(), ws_len, &n_chars_written, nullptr );
        return (int( n_chars_written ) < ws_len? 0 : n);    // Reporting actual count is costly.
    }

    static_assert( Interface::is_implemented_by_<For_windows>() );

    inline auto singleton()
        -> For_windows&
    { return For_windows::singleton(); }

    inline void init() { singleton(); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::singleton,
        d::init;
    }  // namespace exported names
}  // namespace kickstart::utf8_io::standard_streams::_definitions

namespace kickstart::utf8_io::standard_streams     { using namespace _definitions::exported_names; }
