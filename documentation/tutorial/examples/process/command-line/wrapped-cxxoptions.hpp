#pragma once
#include <kickstart/all.hpp>    // <url: https://github.com/alf-p-steinbach/kickstart>
#include <cxxopts.hpp>          // <url: https://github.com/jarro2783/cxxopts>

#include <initializer_list>
#include <optional>
#include <memory>
#include <string>
#include <type_traits>

namespace my {
    namespace ks = kickstart::all;
    using   ks::Array_span_, ks::C_str, ks::str, ks::int_size, ks::begin_ptr_of;
    using   std::initializer_list,      // From <initializer_list>.
            std::move,                  // From <optional>.
            std::shared_ptr,            // From <memory>.
            std::string,                // From <string>
            std::is_same_v;             // From <type_traits>.

    class Options
    {
        mutable cxxopts::Options    m_cxxopts;

        using V = cxxopts::Value;

        template< class Type >
        static auto value() -> shared_ptr<V> { return cxxopts::value<Type>(); }

    public:
        struct Spec
        {
            string                      m_options;
            string                      m_description;
            std::shared_ptr<const V>    m_value;
            string                      m_arg_help;
        };

        Options(
            string                          appname,
            string                          description,
            const initializer_list<Spec>    specs = {}
            ):
            m_cxxopts( appname, description )
        {
            for( const auto spec: specs ) {
                m_cxxopts.add_options()(
                    spec.m_options,
                    spec.m_description,
                    (spec.m_value? spec.m_value : ::cxxopts::value<bool>()),
                    spec.m_arg_help
                    );
            }
        }
        
        template< class Type >
        static auto spec_for( const Type v )
            -> string
        {
            using std::is_same_v;
            if constexpr( is_same_v<Type, bool> ) {
                return (v? "true" : "false");
            } else {    // Using `else` suppresses an MSVC “unreachable code” warning.
                return str( v );
            }
        }

        // Just specifies the kind of option.
        template< class Type >
        static auto kind()
            -> shared_ptr<V>
        { return value<Type>(); }

        // Specifies the result when an option is not present in the command line.
        template< class Type >
        static auto no_option_default( const Type v )
            -> shared_ptr<V>
        { return value<Type>()->default_value( spec_for( v ) ); }

        // Specifies the result when an option is present but sans value spec.
        template< class Type >
        static auto no_value_default( const Type v )
            -> shared_ptr<V>
        { return value<Type>()->implicit_value( spec_for( v ) ); }

        auto parse( const Array_span_<const C_str> parts ) const
            -> cxxopts::ParseResult
        { return m_cxxopts.parse( int_size( parts ), begin_ptr_of( parts ) ); }
        
        auto help_text() const
            -> string
        { return m_cxxopts.help(); }
    };
}  // namespace my
