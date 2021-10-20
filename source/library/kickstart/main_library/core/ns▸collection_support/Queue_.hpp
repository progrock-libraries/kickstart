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

#include <kickstart/main_library/core/ns▸collection_support/collection-iteration.hpp>
#include <kickstart/main_library/core/ns▸language/syntax-support/KS_IT_RANGE_OF.hpp>

#include <queue>
#include <utility>

namespace kickstart::tag {
    using With_items = struct With_items_struct*;
    constexpr auto with_items = With_items();
}  // namespace kickstart::tag

namespace kickstart::collection_support::_definitions {
    using   std::queue,
            std::move;

    template< class Value >
    class Queue_ :
        public queue<Value>
    {
        using Base = queue<Value>;

    public:
        using Container = typename Base::container_type;
        using queue<Value>::queue;      // Constructors.

        template< class InputIt >
        Queue_( const InputIt first, const InputIt last )
        {
            for( const Value& v : all_of( first, last ) ) {
                Base::push( v );
            }
        }

        template< class Items_container >
        Queue_( tag::With_items, const Items_container& items )
            : Queue_( KS_IT_RANGE_OF( items ) )
        {}

        auto popped_front()
            -> Value
        {
            Value result = move( Base::front() );
            Base::pop();
            return result;
        }

        auto items() const
            -> const Container&
        { return Base::c; }
    };

    namespace d = _definitions;
    namespace exported_names {
        using   d::Queue_;
    }  // namespace exported_names
}  // namespace kickstart::collection_support::_definitions

namespace kickstart::collection_support { using namespace _definitions::exported_names; }
