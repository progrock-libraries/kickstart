// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/assertion-headers/~assert-reasonable-compiler.hpp>

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

#include <kickstart/main_library/system-specific/os-detection.hpp>
#if defined( KS_OS_IS_WIN64 )
#   include <kickstart/main_library/system-specific/windows/get_path_of_executable.impl.hpp>
#elif defined( KS_OS_IS_LINUX )
#   include <kickstart/main_library/system-specific/unix/linux/get_path_of_executable.impl.hpp>
#elif defined( KS_OS_IS_UNIX )
#   include <kickstart/main_library/system-specific/unix/get_path_of_executable.impl.hpp>
#else
#   include <kickstart/main_library/core/failure-handling.hpp>      // KS_FAIL
    namespace kickstart::system_specific::_definitions {
        using std::string;

        inline auto get_path_of_executable()
            -> string
        {
            KS_FAIL( "This platform is not supported." );
            unreachable();
        }
    }  // namespace kickstart::system_specific::_definitions
#endif

#include <kickstart/main_library/system-specific/get_path_of_executable.interface.hpp>
