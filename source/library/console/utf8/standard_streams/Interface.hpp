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

#include <kickstart/core/language/type-aliases.hpp>

#include <stdio.h>      // FILE*

#include <type_traits>
#include <tuple>

namespace kickstart::utf8_io::standard_streams {
    using   std::is_same_v;         // From <type_traits>.
    using namespace language;       // `Size` etc.

    namespace _definitions {
        using std::tuple;               // From <tuple>;

        template< class Member_func >
        struct Member_func_signature_t_;

        template< class Ret, class C, class... Args >
        struct Member_func_signature_t_< auto (C::*)( Args... ) const -> Ret >
        {
            using T = const tuple<Ret, Args...>;
        };

        template< auto member_func_ptr >
        using Signature_ = typename Member_func_signature_t_< decltype( member_func_ptr ) >::T;
    }  // namespace _definitions

    class Interface
    {
    public:
        using Self = Interface;

        struct Func
        {
            using Read_byte = auto (FILE*) -> int;                          // Like `fgetc`.
            using Write     = auto (const void*,  Size, FILE*) -> Size;     // ~Like `fwrite`.
        };

        auto read_byte_func_for( FILE* ) const -> Func::Read_byte&;
        auto write_func_for( FILE* ) const -> Func::Write&;

        template< class Impl >
        constexpr static auto is_implemented_by_()
            -> bool
        {
            using _definitions::Signature_;
            return true
                and is_same_v<
                    Signature_< &Impl::read_byte_func_for >,
                    Signature_< &Self::read_byte_func_for >
                    >
                and is_same_v<
                    Signature_< &Impl::write_func_for >,
                    Signature_< &Self::write_func_for >
                    >;
        }
    };

#if 0
    inline auto singleton() -> Impl&;
    inline void init() { singleton>(); }  // Just for readability when other stuff is not used.
#endif

}  // namespace kickstart::utf8_io::standard_streams
