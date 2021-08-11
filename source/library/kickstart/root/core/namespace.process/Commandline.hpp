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

#include <kickstart/root/core/stdlib-extensions/collections/Array_span_.hpp>    // Array_span_
#include <kickstart/root/core/failure-handling.hpp>
#include <kickstart/root/core/namespace.language/types/type-aliases.hpp>                  // C_str
#include <kickstart/root/core/text-encoding/namespace.ascii/character-util.hpp>
#include <kickstart/root/system-specific/get_commandline_data.hpp>              // get_command_line_data

#include <kickstart/root/system-specific/os-detection.hpp>
#ifdef KS_OS_IS_WIN64
#   include <kickstart/root/system-specific/windows/text-encoding-conversion.hpp>
#endif

#include <kickstart/c/assert.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace kickstart::process::_definitions {
    using namespace kickstart::collection_utility;  // begin_ptr_of, end_ptr_of
    using namespace kickstart::failure_handling;    // hopefully, fail
    using namespace kickstart::language;            // Type_, C_str
    using namespace std::string_view_literals;

    namespace ascii = kickstart::ascii;

    using   kickstart::collections::Array_span_,
            kickstart::system_specific::Commandline_data,
            kickstart::system_specific::get_commandline_data;
    using   std::unique_ptr,
            std::string,
            std::string_view,
            std::move,                  // From <utility>
            std::vector;

    inline void assert_good_enough_commandline_data(
        const int                   n_parts,
        const Type_<const C_str*>   parts
        )
    {
        assert( n_parts > 0 );
        assert( parts != nullptr );
        #ifdef KS_OS_IS_WIN64
        {
            for( int i = 0; i < n_parts; ++i ) {
                hopefully( system_specific::is_valid_utf8( parts[i] ) )
                    or KS_FAIL_( std_exception::invalid_argument, "Invalid as UTF-8-encoded text." );
            }
        }
        #endif
        (void) n_parts;  (void) parts;
    }

    struct Assert_good_enough_commandline_data
    {
        Assert_good_enough_commandline_data() {}

        Assert_good_enough_commandline_data(
            const int                               n_parts,
            const Type_<const Type_<const char*>*>  parts
            )
        { assert_good_enough_commandline_data( n_parts, parts ); }
    };

    class Commandline_impl:
        private Assert_good_enough_commandline_data
    {
        using Self = Commandline_impl;
        Commandline_impl( const Self& ) = delete;
        auto operator=( const Self& ) -> Self& = delete;

        Commandline_data        m_data;
        vector<C_str>           m_c_strings;

        void init_c_strings()
        {
            assert( m_c_strings.empty() );
            m_c_strings.reserve( m_data.parts.size() );
            for( const string& s: m_data.parts ) {
                m_c_strings.push_back( s.c_str() );
            }
        }

        Commandline_impl():
            m_data( get_commandline_data() )
        {
            init_c_strings();
        }

        Commandline_impl(
            const int                   n_parts,
            const Type_<const C_str*>   parts,
            const C_str                 fulltext_spec   // = nullptr
            ):
            Assert_good_enough_commandline_data( n_parts, parts ),
            m_data{ fulltext_spec? fulltext_spec : "", vector<string>( parts, parts + n_parts ) }
        {
            init_c_strings();
            if( not fulltext_spec ) {
                #if defined( KS_OS_IS_WIN64 )
                    const char escape = '^';
                #elif defined( KS_OS_IS_UNIX )
                    const char escape = '\\';
                #else
                    const char escape = '\0';
                #endif

                // Synthesize a (for Windows and Unix valid, if ugly) fulltext command line.
                // TODO: Generalize and fix for Windows. Current code may not not quote enough.
                for( int i = 0; i < n_parts; ++i ) {
                    for( const char* p = parts[i]; *p; ++p ) {
                        const char ch = *p;
                        const auto npos = string_view::npos;
                        if constexpr( !!escape ) {
                            if( false
                                or "<>()&|,;\"\'"sv.find( ch ) != npos
                                or ch == escape
                                or is( ascii::whitespace, ch )
                                ) {
                                m_data.fulltext += escape;
                            }
                        }
                        m_data.fulltext += ch;
                    }
                }
            }
        }

        static inline auto new_or_existing_singleton(
            const int                   n_parts,
            const Type_<const C_str*>   parts,
            const C_str                 fulltext    = nullptr
            ) -> const Commandline_impl&
        {
            static unique_ptr<Commandline_impl> p_the_instance = nullptr;

            // TODO: correctness: add mutex; speed: fast path optimization for already existing.
            hopefully( n_parts == 0 or not p_the_instance )
                or KS_FAIL( "Creation arguments specified for access of existing singleton." );
            if( not p_the_instance ) {
                p_the_instance = unique_ptr<Commandline_impl>(
                    n_parts == 0? new Commandline_impl() : new Commandline_impl( n_parts, parts, fulltext )
                    );
            }
            return *p_the_instance;
        }

    public:
        auto fulltext() const   -> const string&    { return m_data.fulltext; }
        auto verb() const       -> const string&    { return m_data.parts[0]; }

        auto args() const
            -> Array_span_<const string>
        { return {begin_ptr_of( m_data.parts ) + 1, end_ptr_of( m_data.parts )}; }

        auto c_strings() const
            -> Array_span_<const C_str>
        { return {begin_ptr_of( m_c_strings ), end_ptr_of( m_c_strings )}; }

        static inline void create_singleton(
            const int                   n_parts,
            const Type_<const C_str*>   parts,
            const C_str                 fulltext_spec   = nullptr
            )
        {
            assert( n_parts > 0 );
            new_or_existing_singleton( n_parts, parts, fulltext_spec );
        }

        static inline auto singleton()
            -> const Commandline_impl&
        { return new_or_existing_singleton( 0, nullptr, nullptr ); }
    };

    class Commandline
    {
        using Self = Commandline_impl;

        Commandline() {}
        Commandline( const Self& ) = delete;
        auto operator=( const Self& ) -> Self& = delete;

        static auto impl()
            -> const Commandline_impl&
        { return Commandline_impl::singleton(); }

    public:
        auto fulltext() const   -> const string&                { return impl().fulltext(); }
        auto verb() const       -> const string&                { return impl().verb(); }
        auto args() const       -> Array_span_<const string>    { return impl().args(); }
        auto c_strings() const  -> Array_span_<const C_str>     { return impl().c_strings(); }

        operator const string& () const { return fulltext(); }

        static inline void create_singleton(
            const int                   n_parts,
            const Type_<const C_str*>   parts,
            const C_str                 fulltext_spec   = nullptr
            )
        { Commandline_impl::create_singleton( n_parts, parts, fulltext_spec ); }

        static inline auto singleton()
            -> const Commandline&
        {
            static Commandline the_instance;
            return the_instance;
        }
    };

    inline auto the_commandline()
        -> const Commandline&
    { return Commandline::singleton(); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::assert_good_enough_commandline_data,
        d::Commandline,
        d::the_commandline;
    }  // namespace exported_names
}  // namespace kickstart::process::_definitions

namespace kickstart::process        { using namespace _definitions; }
