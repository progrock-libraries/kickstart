// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#ifdef _WIN64
#   error "This header is not for 64-bit Windows systems."
#endif
#include "../../../assertion-headers/assert-is-c++17-or-later.hpp"
#include "../../../assertion-headers/assert-utf8-literals.hpp"

// Copyright (c) 2020 Alf P. Steinbach. MIT license, with license text:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Interface.hpp"
#include "../../type_aliases.hpp"

namespace kickstart::utf8::standard_streams::_definitions  {
    using namespace type_aliases;       // `Size` etc.

    class For_utf8_systems
    {
        struct C_streams
        {
            static auto read_byte( FILE* f )
                -> int
            { return ::fgetc( f ); }

            static auto write( const void* buffer, const Size buffer_size, FILE* f)
                -> Size
            { return ::fwrite( buffer, 1, buffer_size, f ); }
        };

    public:
        using Func = standard_streams::Interface::Func;

        auto read_byte_func_for( FILE* ) const -> Func::Read_byte& { return *C_streams::read_byte; }
        auto write_func_for( FILE* ) const -> Func::Write& { return *C_streams::write; }
    };

    static_assert( Interface::is_implemented_by_<For_utf8_systems>() );

    inline auto singleton()
        -> For_utf8_systems&
    {
        static For_utf8_systems the_instance;
        return the_instance;
    }

    inline void init() { singleton(); }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::singleton,
        d::init;
    }  // namespace exported names
}  // namespace kickstart::utf8::standard_streams::_definitions

namespace kickstart::utf8::standard_streams     { using namespace _definitions::exported_names; }
