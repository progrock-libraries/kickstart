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
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/system-specific/get_path_of_executable.hpp>

#include <stddef.h>         // size_t

#include <filesystem>
#include <string>
#include <string_view>
#include <utility>

namespace tag {
    using From_fspath = struct From_fspath_struct*;
}  // namespace tag

namespace kickstart::fsx::_definitions {
    namespace fs = std::filesystem;
    using   kickstart::language::Truth;
    using   std::string,
            std::string_view,
            std::move;

    class Path;
    inline auto fspath_from_u8( const string_view& s ) -> fs::path;
    inline auto path_from_u8( const string_view& s ) -> Path;

    inline auto fspath_of_executable() -> fs::path;
    inline auto path_of_executable() -> Path;

    inline auto fspath_of_exe_directory() -> fs::path;
    inline auto path_of_exe_directory() -> Path;

    // Like `absolute` except instead of the current directory the exe directory is used.
    inline auto fs_fullpath_of( const fs::path& ) -> fs::path;
    inline auto fullpath_of( const Path& ) -> Path;

    class Path
    {
        fs::path        m_value;

    public:
        using Const_iterator    = fs::path::iterator;
        using Iterator          = Const_iterator;

        Path() noexcept {}

        Path( const string_view& s ): m_value( fspath_from_u8( s ) ) {}

        #ifdef __cpp_lib_char8_t
            using std::u8string_view;
            Path( const u8string_view& s ): m_value( s ) {}
        #endif

        Path( Path&& other ) noexcept:
            m_value( move( other.m_value ) )
        {}

        Path( tag::From_fspath, fs::path p ) noexcept:
            m_value( move( p ) )
        {}

        static auto from_fspath( fs::path p )
            -> Path
        { return Path( tag::From_fspath(), move( p ) ); }

        auto fspath() -> fs::path& { return m_value; }
        auto fspath() const -> const fs::path& { return m_value; }


        auto operator-() const -> const fs::path& { return fspath(); }      // Workaround for g++.
        operator const fs::path& () const { return fspath(); }              // Should work for std.


        //--------------------------- Modifiers:

        auto operator=( const Path& other )
            -> Path&
        {
            m_value = other.m_value;
            return *this;
        }

        auto operator=( Path&& other ) noexcept
            -> Path&
        {
            m_value = move( other.m_value );
            return *this;
        }

        void replace_filename( const Path& new_name ) 
        {
            m_value.replace_filename( new_name );
        }

        void replace_extension( const Path& new_ext )
        {
            m_value.replace_extension( new_ext );
        }


        //--------------------------- Queries:

        auto has_root_name() const              -> Truth    { return m_value.has_root_name(); }
        auto has_root_directory() const         -> Truth    { return m_value.has_root_directory(); }
        auto has_root_path() const              -> Truth    { return m_value.has_root_path(); }
        auto has_relative_path() const          -> Truth    { return m_value.has_relative_path(); }
        auto has_parent_path() const            -> Truth    { return m_value.has_parent_path(); }
        auto has_filename() const               -> Truth    { return m_value.has_filename(); }
        auto has_filename_mainpart() const      -> Truth    { return m_value.has_stem(); }                      // ¤
        auto has_filename_extension() const     -> Truth    { return m_value.has_extension(); }                 // ¤

        auto root_name() const                  -> Path     { return from_fspath( m_value.root_name() ); }
        auto root_directory() const             -> Path     { return from_fspath( m_value.root_directory() ); }
        auto root_path() const                  -> Path     { return from_fspath( m_value.root_path() ); }
        auto relative_path() const              -> Path     { return from_fspath( m_value.relative_path() ); }
        auto parent_path() const                -> Path     { return from_fspath( m_value.parent_path() ); }
        auto filename() const                   -> Path     { return from_fspath( m_value.filename() ); }
        auto filename_mainpart() const          -> Path     { return from_fspath( m_value.stem() ); }           // ¤
        auto filename_extension() const         -> Path     { return from_fspath( m_value.extension() ); }      // ¤

        auto is_absolute() const                -> Truth    { return m_value.is_absolute(); }
        auto is_relative() const                -> Truth    { return m_value.is_relative(); }

        auto is_empty() const noexcept                                                                          // ¤
            -> Truth
        { return m_value.empty(); }

        auto to_string() const 
            -> string
        {
            const auto s = m_value.u8string();      // In C++20 typed to sabotage Windows
            return string( s.begin(), s.end() );    // ... incurring this inefficiency.
        }


        //--------------------------- Iterators:

        auto begin() const      -> Iterator     { return m_value.begin(); }
        auto end() const        -> Iterator     { return m_value.end(); }


        //--------------------------- Misc:

        friend
        auto operator/( const Path& lhs, const Path& rhs )
            -> Path
        { return from_fspath( lhs.m_value / rhs.m_value ); }
    };

    inline void swap( Path& a, Path& b ) noexcept
    {
        swap( a.fspath(), b.fspath() );
    }

    inline auto hash_value( const Path& path ) noexcept
        -> size_t
    { return fs::hash_value( path.fspath() ); }

    inline auto compare( const Path& a, const Path& b )
        -> int
    { return a.fspath().compare( b.fspath() ); }

    inline auto operator<( const Path& lhs, const Path& rhs ) noexcept
        -> auto
    { return compare( lhs, rhs ) < 0; }

    inline auto operator<=( const Path& lhs, const Path& rhs ) noexcept
        -> auto
    { return compare( lhs, rhs ) <= 0; }

    inline auto operator==( const Path& lhs, const Path& rhs ) noexcept
        -> auto
    { return compare( lhs, rhs ) == 0; }

    inline auto operator>=( const Path& lhs, const Path& rhs ) noexcept
        -> auto
    { return compare( lhs, rhs ) >= 0; }

    inline auto operator>( const Path& lhs, const Path& rhs ) noexcept
        -> auto
    { return compare( lhs, rhs ) > 0; }

    inline auto operator!=( const Path& lhs, const Path& rhs ) noexcept
        -> auto
    { return compare( lhs, rhs ) != 0; }

    // Sabotage of Windows: this function may stop working in C++23 because
    // `std::filesystem::u8path` is deprecated in C++20. May be necessary to
    // re-implement using OS functionality and/or C++ version discrimination.
    inline auto fspath_from_u8( const string_view& s )
        -> fs::path
    { return fs::u8path( s ); }

    inline auto path_from_u8( const string_view& s )
        -> Path
    { return Path::from_fspath( fspath_from_u8( s ) ); }

    inline auto fspath_of_executable()
        -> fs::path
    { return path_from_u8( kickstart::system_specific::get_path_of_executable() ); }

    inline auto path_of_executable()
        -> Path
    { return Path::from_fspath( fspath_of_executable() ); }

    inline auto fspath_of_exe_directory()
        -> fs::path
    { return fspath_of_executable().parent_path(); }

    inline auto path_of_exe_directory()
        -> Path
    { return Path::from_fspath( fspath_of_exe_directory() ); }

    // Like `absolute` except instead of the current directory the executable's directory is used.
    inline auto fs_fullpath_of( const fs::path& p )
        -> fs::path
    { return (p.is_relative()? (fspath_of_executable() / p) : fs::absolute( p )); }

    inline auto fullpath_of( const Path& p )
        -> Path
    { return Path::from_fspath( fs_fullpath_of( p ) ); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::fspath_from_u8, d::path_from_u8,
        d::fspath_of_executable, d::path_of_executable,
        d::fspath_of_exe_directory, d::path_of_exe_directory,
        d::Path,
        d::swap,
        d::hash_value,
        d::compare,
        d::operator<, d::operator<=, d::operator==, d::operator>=, d::operator>, d::operator!=;
    }  // namespace exported names
}  // namespace kickstart::fsx::_definitions

namespace kickstart::fsx        { using namespace _definitions::exported_names; }
namespace kickstart::core       { namespace fsx = kickstart::fsx; }
