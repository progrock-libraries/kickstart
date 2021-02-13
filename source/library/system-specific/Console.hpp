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

#include <string>
#include <string_view>

namespace kickstart::system_specific::_definitions {
    using   std::string,
            std::string_view;

    class Console
    {
    protected:
        Console() {}

    public:
        static inline auto instance() -> Console&;

        virtual auto input() -> string = 0;
        virtual void output( const string_view& ) = 0;

        auto input( const string_view& prompt )
            -> string
        {
            output( prompt );
            return input();
        }
    };

    inline auto the_console()
        -> Console&
    { return Console::instance(); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Console,
        d::the_console;
    }  // namespace exported_names
}  // namespace kickstart::system_specific::_definitions

namespace kickstart::system_specific    { using namespace _definitions::exported_names; }

#if defined( _WIN64 )
#   include <kickstart/system-specific/windows/Console.impl.hpp>
#elif defined( x__linux__ )
#   include <kickstart/system-specific/unix/linux/Console.impl.hpp>
#else
#   include <kickstart/core/failure-handling.hpp>
    namespace kickstart::system_specific::_definitions {
        using kickstart::failure_handling::unreachable;

        inline auto Console::instance()
            -> Console&
        {
            KS_FAIL( "This platform is not supported." );
            unreachable();
        }
    }  // namespace kickstart::system_specific::_definitions
#endif
