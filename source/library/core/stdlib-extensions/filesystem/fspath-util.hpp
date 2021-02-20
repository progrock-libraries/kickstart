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

#include <kickstart/core/failure-handling.hpp>
//#include <kickstart/core/language/type-aliases.hpp>
#include <kickstart/core/stdlib-extensions/strings.hpp>             // C_str_ref
#include <kickstart/system-specific/get_path_of_executable.hpp>
#include <kickstart/system-specific/u8open_c_file.hpp>

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace kickstart::fsx::_definitions {
    namespace fs = std::filesystem;
    namespace ks = kickstart::system_specific;

    using   kickstart::c_files::C_file;
    using   kickstart::strings::C_str_ref;
    using   std::optional,
            std::string,
            std::string_view;

    struct Explicit_fspath
    {
        const fs::path&     ref;

        Explicit_fspath( const fs::path& arg ):
            ref( arg )
        {}
    };

    // Sabotage of Windows: this function may stop working in C++23 because
    // `std::filesystem::u8path` is deprecated in C++20. May be necessary to
    // re-implement using OS functionality and/or C++ version discrimination.
    inline auto fspath_from_u8( const string_view& s )
        -> fs::path
    { return fs::u8path( s ); }

    inline auto u8_from( const fs::path& p )
        -> string
    {
        const auto s = p.u8string();            // In C++20 typed to sabotage Windows
        return string( s.begin(), s.end() );    // ... incurring this inefficiency.
    }

    inline auto fspath_of_executable()
        -> fs::path
    { return fspath_from_u8( ks::get_path_of_executable() ); }

    inline auto fspath_of_exe_directory()
        -> fs::path
    { return fspath_of_executable().parent_path(); }

    // Like `absolute` except instead of the current directory the executable's
    // directory is used.
    inline auto full_fspath_of( const fs::path& p )
        -> fs::path
    { return (p.is_relative()? (fspath_of_executable() / p) : fs::absolute( p )); }

    inline auto open_c_file( const Explicit_fspath p, const C_str_ref mode )
        -> optional<C_file>
    { return ks::u8open_c_file( u8_from( p.ref ), mode ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::fspath_from_u8,
        d::u8_from,
        d::fspath_of_executable,
        d::fspath_of_exe_directory,
        d::full_fspath_of,
        d::open_c_file;
    }  // namespace exported names
}  // namespace kickstart::fsx::_definitions

namespace kickstart::fsx    { using namespace _definitions::exported_names; }
