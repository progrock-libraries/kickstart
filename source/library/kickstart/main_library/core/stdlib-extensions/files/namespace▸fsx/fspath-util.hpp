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

#include <kickstart/main_library/core/failure-handling.hpp>
//#include <kickstart/main_library/core/namespace▸language/types/type-aliases.hpp>
#include <kickstart/main_library/core/stdlib-extensions//namespace▸strings.hpp>             // C_str_ref
#include <kickstart/main_library/system-specific/get_path_of_executable.hpp>
#include <kickstart/main_library/system-specific/open-c-file.hpp>

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
        const fs::path& ref;
        Explicit_fspath( const fs::path& arg ): ref( arg ) {}
    };

    // Sabotage of Windows: this function may stop working in C++23 because
    // `std::filesystem::u8path` is deprecated in C++20. May be necessary to
    // re-implement using OS functionality and/or C++ version discrimination.
    inline auto fspath_from_u8( const string_view& s )
        -> fs::path
    { return fs::u8path( s ); }

    inline auto u8_from( const Explicit_fspath p )
        -> string
    {
        const auto s = p.ref.u8string();        // In C++20 typed to sabotage Windows
        return string( s.begin(), s.end() );    // ... incurring this inefficiency.
    }

    inline auto exe_file_fspath()
        -> const fs::path&
    {
        static const fs::path the_exe_path = fspath_from_u8( ks::get_path_of_executable() );
        return the_exe_path;
    }

    inline auto exe_directory_fspath()
        -> const fs::path&
    {
        static const fs::path the_exedir_path = exe_file_fspath().parent_path();
        return the_exedir_path;
    }

    // Like `absolute` except instead of the current directory the executable's
    // directory is used.
    inline auto exe_relative_fspath( const Explicit_fspath p )
        -> fs::path
    { return (p.ref.is_relative()? (exe_directory_fspath() / p.ref) : fs::absolute( p.ref )); }

    inline auto open_c_file( const Explicit_fspath p, const C_str_ref mode )
        -> optional<C_file>
    { return ks::open_c_file( p.ref, mode.s() ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Explicit_fspath,
        d::fspath_from_u8,
        d::u8_from,
        d::exe_file_fspath,
        d::exe_directory_fspath,
        d::exe_relative_fspath,
        d::open_c_file;
    }  // namespace exported names
}  // namespace kickstart::fsx::_definitions

namespace kickstart::fsx    { using namespace _definitions::exported_names; }
