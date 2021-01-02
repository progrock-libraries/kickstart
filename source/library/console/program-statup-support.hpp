// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../assertion-headers/assert-reasonable-compiler.hpp"

// kickstart.core.hpp - minimal convenience functionality for C++ learners.
// The “core” is because the (rather large) iostreams headers are not included.
//
// Requires C++17 or later.

// Copyright (c) 2020 Alf P. Steinbach. MIT license, with license text:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "../core/language/type_aliases.hpp"    // Type_, C_str
#include "../core/failure-handling.hpp"         // Clean_app_exception
#include "utf8/io.hpp"                          // output_error_message

#include <assert.h>         // assert
#include <stdlib.h>         // EXIT_..., strtod
#include <string.h>         // strerror

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace kickstart::startup::_definitions {
    using namespace kickstart::failure_handling;
    using namespace kickstart::language;
    using namespace std::string_literals;

    using   kickstart::utf8_io::output_error_message;
    using   std::function,
            std::string_view,
            std::vector;

    using Simple_startup        = void();
    using Startup_with_args     = void( const string_view&, const vector<string_view>& );

    inline auto with_exceptions_displayed( const function<Simple_startup>& do_things )
        -> int
    {
        try{
            do_things();
            return EXIT_SUCCESS;
        } catch( const Clean_app_exit_exception& x ) {
            output_error_message( ""s + x.what() + "\n" );
        } catch( const exception& x ) {
            output_error_message( "!"s + x.what() + "\n" );
        }
        return EXIT_FAILURE;
    }

    inline auto with_exceptions_displayed(
        const function<Startup_with_args>&      do_things,
        const int                               n_cmd_parts,
        const Type_<const C_str*>               cmd_parts
        ) -> int
    {
        assert( n_cmd_parts >= 1 );
        assert( cmd_parts != nullptr );
        return with_exceptions_displayed( [&]() -> void
        {
            do_things(
                cmd_parts[0],
                vector<string_view>( cmd_parts + 1, cmd_parts + n_cmd_parts )
            );
        } );
    }

    inline auto with_exceptions_displayed( const function<Startup_with_args>& do_things )
        -> int
    {
        const C_str dummy_cmd[] = {"", nullptr};
        return with_exceptions_displayed( do_things, 1, dummy_cmd );
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Simple_startup,
        d::Startup_with_args,
        d::with_exceptions_displayed;
    }  // namespace exported names
}  // namespace kickstart::startup::_definitions

namespace kickstart::startup    { using namespace _definitions::exported_names; }
namespace kickstart::core       { using namespace startup; }
