// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include "../../assertion-headers/$-assert-reasonable-compiler.hpp"

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

#include "../../core/language/type_aliases.hpp"
#include "../../system-specific/windows/api/process-startup-info.hpp"
#include "Commandline.interface.hpp"

#include <assert.h>

#include <vector>
#include <memory>

namespace kickstart::process::_definitions {
    using   std::string,
            std::vector,
            std::unique_ptr, std::make_unique;

    using namespace kickstart::failure_handling;    // hopefully, fail
    using namespace kickstart::language;            // Type_

    class Windows_commandline
        : public Commandline
    {
        friend static auto Commandline::singleton() -> const Commandline&;

        using Self = Windows_commandline;
        using Raw_parts = unique_ptr< Type_<wchar_t*>[], Type_<void(wchar_t**)>* >;

        Windows_commandline( const Self& ) = delete;
        auto operator=( const Self& ) -> Self& = delete;

        static void free_parts( const Type_<wchar_t**> p ) { winapi::LocalFree( p ); }

        string                  m_command_line;
        vector<string>          m_parts;
        vector<string_view>     m_part_views;

        Windows_commandline()
        {
            namespace winapi = kickstart::winapi;
            const Type_<wchar_t*> command_line = winapi::GetCommandLineW();
            int n_parts = 0;
            auto raw_parts = Raw_parts( nullptr, free_parts );
            raw_parts.reset( winapi::CommandLineToArgvW( command_line, &n_parts ) );
            hopefully( raw_parts != nullptr )
                or KS_FAIL( "Failed to split command line into parts (::CommandLineToArgvW failed)" );
            assert( n_parts >= 1 );

            //m_part_views = vector<string_view>( m_parts.get(), m_parts.get() + n_parts );
        }


    public:
        auto fulltext() const -> string_view override   { return m_command_line; }
        auto verb() const -> string_view override       { return m_part_views[0]; }

        auto args() const
            -> Array_span_<const string_view>
            override
        { return {&m_part_views[1], &m_part_views.back()}; }
    };

    inline auto Commandline::singleton()
        -> const Commandline&
    {
        static Windows_commandline the_instance;
        return the_instance;
    }

}  // namespace kickstart::process::_definitions
