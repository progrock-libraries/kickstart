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

namespace kickstart::matrices::_definitions {

    template< class Item_type_param >
    class Abstract_matrix_ref_:
        public Abstract_matrix_<Item_type_param>
    {
    public:
        using Item = Item_type_param;

    private:
        Matrix_<Item_type_param>*   m_p_matrix;

    public:
        Abstract_matrix_ref_( Matrix_<Item>& m ):
            m_p_matrix( &m )
        {}

        operator Abstract_matrix_ref_<const Item>& () const
        {
            return reinterpret_cast<Abstract_matrix_ref_<const Item>&>( *this );
        }

        virtual auto size() const       -> two_d_grid::Size     { return m_p_matrix->size(); }
        virtual auto p_items()          -> Item*                { return m_p_matrix->p_items(); }
        virtual auto p_items() const    -> const Item*          { return m_p_matrix->p_items(); }
    };

    template< class Item_type_param >
    auto Matrix_<Item_type_param>::abstract_ref()
        -> Abstract_matrix_ref_<Item_type_param>
    { return Abstract_matrix_ref_<Item_type_param>( *this ); }

    template< class Item_type_param >
    auto Matrix_<Item_type_param>::abstract_ref() const
        -> Abstract_matrix_ref_<const Item_type_param>
    {
        auto& const_item_m = static_cast<Matrix_<const Item>&>( *this );
        return Abstract_matrix_ref_<const Item_type_param>( const_item_m );
    }


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names {
    }  // namespace exported names
}  // namespace kickstart::matrices::_definitions

namespace kickstart::matrices   { using namespace _definitions::exported_names;}
