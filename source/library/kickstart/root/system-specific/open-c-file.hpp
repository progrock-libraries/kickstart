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

#include <kickstart/root/system-specific/os-detection.hpp>
#if defined( KS_OS_IS_UNIX ) || defined( KS_USE_FOPEN_PLEASE )
#
#   include <kickstart/root/core/failure-handling.hpp>
#   include <kickstart/root/core/namespace▸language/types/type-aliases.hpp>
#   include <kickstart/root/core/stdlib-extensions/c-files/clib-file-types.hpp>
#   include <filesystem>
#   include <kickstart/c/stdio.hpp>

    namespace kickstart::system_specific::_definitions {
        using namespace kickstart::failure_handling;
        using namespace kickstart::language;
        namespace fs = std::filesystem;
        using kickstart::c_files::C_file;

        inline auto raw_u8open_c_file( const C_str utf8_path, const C_str mode )
            -> C_file
        { return ::fopen( utf8_path, mode ); }

        inline auto raw_open_c_file( const fs::path& path, const C_str mode )
            -> C_file
        { return ::fopen( path.c_str(), mode ); }
    }  // namespace kickstart::system_specific::_definitions
#
#elif defined( KS_OS_IS_WIN64 )
#
#   include <kickstart/root/system-specific/windows/open-c-file.impl.hpp>
#
#elif defined( KS_OS_IS_WIN32 )
#
#   error "32-bit Windows is not supported"
#   include <oops_32_bit_windows_is_not_supported>
#
#else
#
#   include <kickstart/root/core/failure-handling.hpp>
#   include <kickstart/root/core/namespace▸language/types/type-aliases.hpp>
#   include <kickstart/root/core/stdlib-extensions/c-files/clib-file-types.hpp>
#   include <filesystem>
#   include <kickstart/c/stdio.hpp>

    namespace kickstart::system_specific::_definitions {
        using namespace kickstart::failure_handling;
        using namespace kickstart::language;
        namespace fs = std::filesystem;
        using kickstart::c_files::C_file;

        inline auto raw_u8open_c_file( const C_str utf8_path, const C_str mode )
            -> C_file
        {
            (void) utf8_path;  (void) mode;
            KS_FAIL( "This platform is not supported." );
            unreachable();
        }

        inline auto raw_open_c_file( const fs::path& path, const C_str mode )
            -> C_file
        { return ::fopen( path.c_str(), mode ); }
    }  // namespace kickstart::system_specific::_definitions
#
#endif

#include <kickstart/root/system-specific/open-c-file.interface.hpp>
