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

#include <kickstart/root/core/stdlib-extensions/files/c_files/C_file_operations.hpp>
#include <kickstart/root/core/stdlib-extensions/files/c_files/wrapped-clib-io.hpp>
#include <kickstart/root/system-specific/console-adapted-io-functions.hpp>

#include <utility>

namespace kickstart::c_files::_definitions {
    using namespace language;       // Truth
    namespace ks = kickstart::system_specific;

    using   std::exchange,
            std::move;

    class Abstract_c_file
    {
        using Self = Abstract_c_file;
        Abstract_c_file( const Self& ) = delete;
        auto operator=( const Self& ) -> Self& = delete;

        C_file  m_c_file;

    protected:
        ~Abstract_c_file() noexcept
        {
            // The `m_c_file` can be set to 0 by the `release` method and by moving.
            if( m_c_file ) { ::fclose( m_c_file ); }
        }

        Abstract_c_file( const C_file f ) noexcept:
            m_c_file( f )
        {
            assert( m_c_file != nullptr );
        }

        Abstract_c_file( Self&& other ) noexcept:
            m_c_file( exchange( other.m_c_file, {} ) )
        {}

        void assign( Self&& other ) noexcept
        {
            m_c_file = exchange( other.m_c_file, {} );
        }

        auto operator=( Self&& other ) noexcept
            -> Self&
        {
            assign( move( other ) );
            return *this;
        }

        auto c_file() const noexcept
            -> C_file
        { return m_c_file; }

        auto release() noexcept
            -> C_file
        { return exchange( m_c_file, nullptr ); }

        //----------------------------------- Only meaningful for input:

        auto has_passed_eof() const
            -> Truth
        { return !!::feof( m_c_file ); }

        auto input_or_none()
            -> optional<string>
        { return clib_input_or_none_from( c_file() ); }

    public:
        auto in_failstate() const noexcept
            -> Truth
        { return !!::ferror( c_file() ); }
    };

    namespace d = _definitions;
    namespace exports{ using
        d::Abstract_c_file;
    }  // exports
}  // namespace kickstart::c_files::_definitions

namespace kickstart::c_files    { using namespace _definitions::exports; }
