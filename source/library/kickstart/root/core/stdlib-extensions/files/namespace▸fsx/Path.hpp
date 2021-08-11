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

#include <kickstart/root/core/failure-handling.hpp>
#include <kickstart/root/core/namespace▸language/types/Truth.hpp>
#include <kickstart/root/core/namespace▸language/types/type-aliases.hpp>
#include <kickstart/root/core/stdlib-extensions/files/namespace▸fsx/fspath-util.hpp>
#include <kickstart/root/core/namespace▸text_conversion/to-text/string-output-operator.hpp>

#include <kickstart/c/stddef.hpp>       // size_t

#include <string>
#include <string_view>
#include <utility>

namespace kickstart::fsx::_definitions {
    using namespace kickstart::language;            // Truth, C_str
    using namespace kickstart::failure_handling;    // unreachable
    using namespace kickstart::text_conversion;     // ""s, string operator<<

    using   std::string,
            std::string_view,
            std::move;

    class Path
    {
        fs::path        m_value;

    public:
        using Const_iterator    = fs::path::iterator;
        using Iterator          = Const_iterator;

        Path() noexcept {}

        Path( const C_str s ): m_value( fspath_from_u8( s ) ) {}
        Path( const string_view& s ): m_value( fspath_from_u8( s ) ) {}
        Path( const string& s ): m_value( fspath_from_u8( s ) ) {}

        #ifdef __cpp_lib_char8_t
            using std::u8string_view;
            Path( const u8string_view& s ): m_value( s ) {}
        #endif

        Path( fs::path p ) noexcept:
            m_value( move( p ) )
        {}

        Path( const Path& other ):
            m_value( other.m_value )
        {}

        Path( Path&& other ) noexcept:
            m_value( move( other.m_value ) )
        {}

        auto fspath() -> fs::path& { return m_value; }
        auto fspath() const -> const fs::path& { return m_value; }


        auto operator-() const -> const fs::path& { return fspath(); }      // Workaround for g++.
        operator const fs::path& () const { return fspath(); }              // Should work for std.

        auto to_string() const 
            -> string
        { return u8_from( m_value ); }


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


        //--------------------------- Queries (“¤” marks different names than in fs::path):

        auto has_root_name() const          -> Truth    { return m_value.has_root_name(); }
        auto has_root_directory() const     -> Truth    { return m_value.has_root_directory(); }
        auto has_root_path() const          -> Truth    { return m_value.has_root_path(); }
        auto has_relative_path() const      -> Truth    { return m_value.has_relative_path(); }
        auto has_parent_path() const        -> Truth    { return m_value.has_parent_path(); }
        auto has_filename() const           -> Truth    { return m_value.has_filename(); }
        auto has_filename_mainpart() const  -> Truth    { return m_value.has_stem(); }                  // ¤
        auto has_filename_extension() const -> Truth    { return m_value.has_extension(); }             // ¤

        auto root_name() const              -> Path     { return Path( m_value.root_name() ); }
        auto root_directory() const         -> Path     { return Path( m_value.root_directory() ); }
        auto root_path() const              -> Path     { return Path( m_value.root_path() ); }
        auto relative_path() const          -> Path     { return Path( m_value.relative_path() ); }
        auto parent_path() const            -> Path     { return Path( m_value.parent_path() ); }
        auto filename() const               -> Path     { return Path( m_value.filename() ); }
        auto filename_mainpart() const      -> Path     { return Path( m_value.stem() ); }              // ¤
        auto filename_extension() const     -> Path     { return Path( m_value.extension() ); }         // ¤

        auto is_absolute() const            -> Truth    { return m_value.is_absolute(); }
        auto is_relative() const            -> Truth    { return m_value.is_relative(); }

        auto is_empty() const noexcept                                                                  // ¤
            -> Truth
        { return m_value.empty(); }


        //--------------------------- Iterators:

        auto begin() const      -> Iterator     { return m_value.begin(); }
        auto end() const        -> Iterator     { return m_value.end(); }


        //--------------------------- Misc:

        friend
        auto operator/( const Path& lhs, const Path& rhs )
            -> Path
        { return Path( lhs.m_value / rhs.m_value ); }
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

    inline auto path_from_u8( const string_view& s )
        -> Path
    { return Path( fspath_from_u8( s ) ); }

    inline auto exe_file_path()
        -> Path
    { return Path( exe_file_fspath() ); }

    inline auto exe_directory_path()
        -> Path
    { return Path( exe_directory_fspath() ); }

    inline auto exe_relative_path( const Path& p )
        -> Path
    { return Path( exe_relative_fspath( p.fspath() ) ); }

    inline auto open_c_file( const Path& p, const C_str_ref mode )
        -> optional<C_file>
    { return ks::open_c_file( p.fspath(), mode ); }

    inline auto open_c_file_or_x( const Path& path, const C_str_ref mode )
        -> C_file
    {
        if( optional<C_file> f = open_c_file( path, mode ) ) {
            return f.value();
        }
        KS_FAIL( ""s << "Unable to open “" << path.to_string() << "” in mode “" << mode << "”." );
        unreachable();
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::Path,
        d::swap,
        d::hash_value,
        d::compare,
        d::operator<, d::operator<=, d::operator==, d::operator>=, d::operator>, d::operator!=,
        d::path_from_u8,
        d::exe_file_path,
        d::exe_directory_path,
        d::exe_relative_path,
        d::open_c_file,
        d::open_c_file_or_x;
    }  // namespace exported names
}  // namespace kickstart::fsx::_definitions

namespace kickstart::fsx        { using namespace _definitions::exported_names; }
