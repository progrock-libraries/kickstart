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

#include <kickstart/core/language/Truth.hpp>

#include <stddef.h>         // size_t

#include <filesystem>
#include <string>
#include <string_view>
#include <utility>

namespace tag {
    using From_stdlib = struct From_stdlib_struct*;
}  // namespace tag

namespace kickstart::language::fsx::_definitions {
    namespace fs = std::filesystem;
    using   kickstart::language::Truth;
    using   std::string,
            std::string_view,
            std::move;

    // Sabotage of Windows: this function may stop working in C++23 because
    // `std::filesystem::u8path` is deprecated in C++20. May be necessary to
    // re-implement using OS functionality and/or C++ version discrimination.
    inline auto u8_stdpath( const string_view& s )
        -> fs::path
    { return fs::u8path( s ); }

    class Path
    {
        fs::path        m_value;

    public:
        using Const_iterator    = fs::path::iterator;
        using Iterator          = Const_iterator;

        Path() noexcept {}

        Path( const string_view& s ):
            m_value( u8_stdpath( s ) )
        {}

        Path( Path&& other ) noexcept:
            m_value( move( other.m_value ) )
        {}

        Path( tag::From_stdlib, fs::path p ) noexcept:
            m_value( move( p ) )
        {}

        static auto from_stdlib_path( fs::path p )
            -> Path
        { return Path( tag::From_stdlib(), move( p ) ); }

        auto stdlib_path() -> fs::path& { return m_value; }
        auto stdlib_path() const -> const fs::path& { return m_value; }


        auto operator- () const -> const fs::path& { return stdlib_path(); }    // Workaround for g++.
        operator const fs::path& () const { return stdlib_path(); }             // Should work for std.


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

        auto has_root_name() const              -> Truth     { return m_value.has_root_name(); }
        auto has_root_directory() const         -> Truth     { return m_value.has_root_directory(); }
        auto has_root_path() const              -> Truth     { return m_value.has_root_path(); }
        auto has_relative_path() const          -> Truth     { return m_value.has_relative_path(); }
        auto has_parent_path() const            -> Truth     { return m_value.has_parent_path(); }
        auto has_filename() const               -> Truth     { return m_value.has_filename(); }
        auto has_filename_mainpart() const      -> Truth     { return m_value.has_stem(); }
        auto has_filename_extension() const     -> Truth     { return m_value.has_extension(); }

        auto root_name() const                  -> Path     { return from_stdlib_path( m_value.root_name() ); }
        auto root_directory() const             -> Path     { return from_stdlib_path( m_value.root_directory() ); }
        auto root_path() const                  -> Path     { return from_stdlib_path( m_value.root_path() ); }
        auto relative_path() const              -> Path     { return from_stdlib_path( m_value.relative_path() ); }
        auto parent_path() const                -> Path     { return from_stdlib_path( m_value.parent_path() ); }
        auto filename() const                   -> Path     { return from_stdlib_path( m_value.filename() ); }
        auto filename_mainpart() const          -> Path     { return from_stdlib_path( m_value.stem() ); }
        auto filename_extension() const         -> Path     { return from_stdlib_path( m_value.extension() ); }

        auto is_absolute() const                -> Truth     { return m_value.is_absolute(); }
        auto is_relative() const                -> Truth     { return m_value.is_relative(); }

        auto is_empty() const noexcept
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
        { return from_stdlib_path( lhs.m_value / rhs.m_value ); }
    };

    inline void swap( Path& a, Path& b ) noexcept
    {
        swap( a.stdlib_path(), b.stdlib_path() );
    }

    inline auto hash_value( const Path& path ) noexcept
        -> size_t
    { return fs::hash_value( path.stdlib_path() ); }

    inline auto compare( const Path& a, const Path& b )
        -> int
    { return a.stdlib_path().compare( b.stdlib_path() ); }

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


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::u8_stdpath,
        d::Path,
        d::swap,
        d::hash_value,
        d::compare,
        d::operator<, d::operator<=, d::operator==, d::operator>=, d::operator>, d::operator!=;

        using namespace fs;     // All the standard library's fs functions etc., e.g. current_path.
    }  // namespace exported names
}  // namespace kickstart::language::fsx::_definitions

namespace kickstart::language::fsx  { using namespace _definitions::exported_names; }
namespace kickstart::core           { namespace fsx = language::fsx; }
