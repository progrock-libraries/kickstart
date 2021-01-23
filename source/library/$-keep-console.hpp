// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once

//----------------------------------------------------------------------------------------
// Ideally don’t `#include` this header. Instead specify it as a forced include:
//
// • Visual C++ compiler:
//   Use option `/FI "kickstart/stop-at-end.hpp"`.
//
// • g++ and clang compilers:
//   Use option `-include "kickstart/stop-at-end.hpp"`.

//----------------------------------------------------------------------------------------
// Description.
//
// Keeps the console window open when the program finishes.
//
// Advantages over a “stop here please” statement at the end of `main`:
//
// • Reliable in the face of typical beginner's non-empty input buffer.
// • Yields clean, portable main code.
// • When used as a forced include, stops /after/ other static cleanup.
// • In Windows there's no added interaction when the program runs in a provided console.
//
// Generally this is needed only when you run a beginner's program from an IDE in Windows,
// and then in the author's informed opinion a better way is to just use the IDE's
// functionality for retaining the program's console window at the end, e.g. running it
// via Ctrl F5 in Visual Studio, which has the same advantages as listed above.

//----------------------------------------------------------------------------------------
// kickstart.hpp - minimal convenience functionality for C++ learners.
// Requires C++17 or later.
//
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

#include <stdlib.h>     // ::system

#ifdef _WIN32
#   include "system-specific/windows/api/consoles.hpp"          // GetConsoleProcessList
#endif

namespace uuid_59f0e797_cfa5_4452_9c30_3473b888089a {
    #if defined( _WIN32 )
        inline const auto& pause_command = R"(pause)";

        inline auto is_console_owner()
            -> bool
        {
            namespace winapi = kickstart::winapi;
            winapi::DWORD dummy;
            return (winapi::GetConsoleProcessList( &dummy, 1 ) == 1);
        }
    #elif defined( __unix__ )
        inline const auto& pause_command = R"(read -p "? Press ENTER to continue: " dummy)";

        inline auto is_console_owner()
            -> bool
        { return true; }        // TODO: Maybe actually check this, if that's possible.
    #else
    #   error "This header is for Windows and Unix platforms only."
    #endif

    struct Stop_at_end
    {
        ~Stop_at_end() { if( is_console_owner() ) { ::system( pause_command ); } }
        Stop_at_end() {}
    };
    
    inline const Stop_at_end dummy{};
} // namespace uuid_59f0e797_cfa5_4452_9c30_3473b888089a
