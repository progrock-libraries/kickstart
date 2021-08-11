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

#include <kickstart/root/core/namespace▸language/types/Truth.hpp>
#include <kickstart/root/core/stdlib-extensions/files/c_files/wrapped-clib-io.hpp>
#include <kickstart/root/system-specific/console-adapted-io-functions.hpp>

namespace kickstart::c_files::_definitions {
    using namespace language;       // Truth
    namespace ks = kickstart::system_specific;

    class C_file_operations
    {
        using Self = C_file_operations;
        C_file_operations( const Self& ) = delete;
        auto operator=( const Self& ) -> Self& = delete;

        C_file  m_c_file;

    public:
        C_file_operations( const C_file f ) noexcept:
            m_c_file( f )
        {
            assert( m_c_file != nullptr );
        }

        auto c_file() const noexcept
            -> C_file
        { return m_c_file; }

        void close() noexcept
        {
            if( m_c_file ) {
                ::fclose( m_c_file );
                m_c_file = nullptr;
            }
        }

        auto has_passed_eof() const
            -> Truth
        { return !!::feof( m_c_file ); }

        auto in_failstate() const noexcept
            -> Truth
        { return !!::ferror( c_file() ); }

        auto input_or_none()
            -> optional<string>
        { return clib_input_or_none_from( c_file() ); }
    };

    namespace d = _definitions;
    namespace exports{ using
        d::C_file_operations;
    }  // exports
}  // namespace kickstart::c_files::_definitions

namespace kickstart::c_files    { using namespace _definitions::exports; }
