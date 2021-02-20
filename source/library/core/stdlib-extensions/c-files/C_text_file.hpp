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
#include <kickstart/core/stdlib-extensions/c-files/wrapped-clib-io.hpp>
#include <kickstart/system-specific/console-adapted-io-functions.hpp>

#include <utility>

namespace kickstart::c_files::_definitions {
    using namespace language;       // Truth
    namespace ks = kickstart::system_specific;

    using   ks::is_console,
            ks::output_to_console,
            ks::input_from_console;
    using   std::exchange,
            std::move;

    using Output_func   = void ( const C_file f, const string_view& s );
    using Input_func    = auto ( const C_file f ) -> string;

    class Wrapped_c_file
    {
        using Self = Wrapped_c_file;
        Wrapped_c_file( const Self& ) = delete;
        auto operator=( const Self& ) -> Self& = delete;

    protected:
        C_file          m_file;

        ~Wrapped_c_file()
        {
            if( m_file ) { ::fclose( m_file ); }
        }

        Wrapped_c_file( const C_file f ):
            m_file( f )
        {}

        Wrapped_c_file( Self&& other ):
            m_file( exchange( other.m_file, {} ) )
        {}

        void assign( Self&& other ) noexcept
        {
            m_file = exchange( other.m_file, {} );
        }

        auto operator=( Self&& other ) noexcept
            -> Self&
        {
            assign( move( other ) );
            return *this;
        }

        auto in_failstate() const
            -> Truth
        { return !!::ferror( m_file ); }
    };

    class C_text_output:
        public Wrapped_c_file
    {
        Output_func*    m_output_to;

    public:
        C_text_output( const C_file f ):
            Wrapped_c_file( f ),
            m_output_to( is_console( f )? output_to_console : clib_output_to )
        {}

        void output( const string_view& s )
        {
            m_output_to( m_file, s );
        }

        void flush()
        {
            ::fflush( m_file );
        }
    };


    class C_text_input:
        public Wrapped_c_file
    {
        Input_func*     m_input_from;

    public:
        C_text_input( const C_file f ):
            Wrapped_c_file( f ),
            m_input_from( is_console( f )? input_from_console : clib_input_from )
        {}

        auto input()
            -> string
        { return m_input_from( m_file ); }
    };


    namespace d = _definitions;
    namespace exports{
    }  // exports
}  // namespace kickstart::c_file_types::_definitions

namespace kickstart::c_files    { using namespace _definitions::exports; }
