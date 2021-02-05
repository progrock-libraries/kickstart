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

#include <kickstart/system-specific/Commandline_data.hpp>

#include <kickstart/core/failure-handling.hpp>
#include <kickstart/core/language/collection-util.hpp>      // end_ptr_of
#include <kickstart/core/language/type-aliases.hpp>         // Type_
#include <kickstart/system-specific/windows/text-encoding-conversion.hpp>
#include <kickstart/system-specific/windows/api/process-startup-info.hpp>

#include <assert.h>

#include <vector>
#include <memory>

namespace kickstart::system_specific::_definitions {
    using   kickstart::system_specific::to_utf8;
    using   std::string,
            std::vector,
            std::unique_ptr;

    using namespace kickstart::failure_handling;    // hopefully, fail
    using namespace kickstart::language;            // Type_, end_ptr_of

    inline void free_parts( const Type_<wchar_t**> p ) { winapi::LocalFree( p ); }

    inline auto get_commandline_data()
        -> Commandline_data
    {
        namespace winapi = kickstart::winapi;
        using Raw_parts = unique_ptr< Type_<wchar_t*>[], Type_<void(wchar_t**)>* >;

        const Type_<wchar_t*> command_line = winapi::GetCommandLineW();
        int n_parts = 0;
        const auto raw_parts = Raw_parts(
            winapi::CommandLineToArgvW( command_line, &n_parts ),
            free_parts
        );
        hopefully( raw_parts != nullptr )
            or KS_FAIL( "Failed to split command line into parts (::CommandLineToArgvW failed)" );
        assert( n_parts >= 1 );

        Commandline_data result;
        result.fulltext = to_utf8( command_line );
        for( int i = 0; i < n_parts; ++i ) {
            result.parts.push_back( to_utf8( raw_parts[i] ) );
        }
        return result;
    }
}  // namespace kickstart::system_specific::_definitions
