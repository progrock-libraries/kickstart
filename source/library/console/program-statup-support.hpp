// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
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

#ifdef _WIN32
#   include <kickstart/system-specific/windows/api/consoles.hpp>        // GetACP
#endif

#include <kickstart/console/utf8/io.hpp>                        // output_error_message
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/core/language/type-aliases.hpp>             // Type_, C_str
#include <kickstart/core/language/startup-function-types.hpp>   // Simple_startup, Startup_with_args
#include <kickstart/core/text-encoding/ascii/string-util.hpp>   // is_all_ascii
#include <kickstart/core/failure-handling.hpp>                  // Clean_app_exception
#include <kickstart/core/process/Commandline.hpp>               // Commandline

#include <assert.h>         // assert
#include <stdlib.h>         // EXIT_..., strtod
#include <string.h>         // strerror

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace kickstart::console_startup::_definitions {
    namespace process   = kickstart::process;
    namespace std_x     = kickstart::std_x;
    using namespace kickstart::failure_handling;
    using namespace kickstart::language;
    using namespace std::string_literals;

    using   kickstart::utf8_io::output_error_message;
    using   std::function,
            std::string,
            std::string_view,
            std::vector;

    inline void with_command_line_parts(
        const function<Simple_startup>&     do_things,
        const int                           n_cmd_parts,
        const Type_<const C_str*>           cmd_parts,
        const Truth                         override_os_cmdline = false
        )
    {
        #if defined( _WIN32 )
            const Truth is_windows = true;
        #else
            const Truth is_windows = false;
        #endif

        if( override_os_cmdline or not is_windows ) {
            process::Commandline::create_singleton( n_cmd_parts, cmd_parts );
        }
        do_things();
    }

    inline void with_command_line_parts(
        const function<Startup_with_args>&      do_things,
        const int                               n_cmd_parts,
        const Type_<const C_str*>               cmd_parts,
        const Truth                             override_os_cmdline = false
    )
    {
        const auto simple_do_things = [&do_things]() -> void
        {
            const auto& c = process::Commandline::singleton();  // a.k.a. the_commandline();
            do_things( c.verb(), c.args() );
        };

        with_command_line_parts(
            simple_do_things, n_cmd_parts, cmd_parts, override_os_cmdline
            );
    }

    inline void with_os_command_line_parts( const function<Startup_with_args>& do_things )
    {
        const auto& c = process::Commandline::singleton();  // a.k.a. the_commandline();
        do_things( c.verb(), c.args() );
    }

    inline auto with_exceptions_displayed( const function<Simple_startup>& do_things )
        -> int
    {
        try{
            do_things();
            return EXIT_SUCCESS;
        } catch( const Clean_app_exit_exception& x ) {
            output_error_message( ""s + x.what() + "\n" );
        } catch( const std_x::Base& x ) {
            output_error_message( "!"s + x.what() + "\n" );
        }
        return EXIT_FAILURE;
    }

    // For non-Windows platforms the parts are unconditionally assumed to be UTF-8 encoded.
    // In Windows with OS commandline override specified, parts invalid as UTF-8 will cause
    // a `std::invalid_argument` exception.
    inline auto with_exceptions_displayed(
        const function<Simple_startup>&     do_things,
        const int                           n_cmd_parts,
        const Type_<const C_str*>           cmd_parts,
        const Truth                         override_os_cmdline = false
        ) -> int
    {
        return with_exceptions_displayed( [&]() -> void
        {
            with_command_line_parts(
                do_things, n_cmd_parts, cmd_parts, override_os_cmdline
                );
        } );
    }

    inline auto with_exceptions_displayed( const function<Startup_with_args>& do_things )
        -> int
    {
        return with_exceptions_displayed( [&]() -> void
        {
            with_os_command_line_parts( do_things );
        } );
    }

    // For non-Windows platforms the parts are unconditionally assumed to be UTF-8 encoded.
    // In Windows with OS commandline override specified, parts invalid as UTF-8 will cause
    // a `std::invalid_argument` exception.
    inline auto with_exceptions_displayed(
        const function<Startup_with_args>&      do_things,
        const int                               n_cmd_parts,
        const Type_<const C_str*>               cmd_parts,
        const Truth                             override_os_cmdline = false
        ) -> int
    {
        return with_exceptions_displayed( [&]() -> void
        {
            with_command_line_parts(
                do_things, n_cmd_parts, cmd_parts, override_os_cmdline
                );
        } );
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Simple_startup,
        d::Startup_with_args,
        d::with_command_line_parts,
        d::with_os_command_line_parts,
        d::with_exceptions_displayed;
    }  // namespace exported names
}  // namespace kickstart::console_startup::_definitions

namespace kickstart::console_startup    { using namespace _definitions::exported_names; }
