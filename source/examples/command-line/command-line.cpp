#include <kickstart/all.hpp>
using namespace kickstart::all;
using std::ostringstream;

#include <type_traits>
using std::is_convertible_v;

struct S
{
    operator string_view() const { return "Bah"; }
};

template< class T >
void foo( const T& v )
{
     if constexpr( is_convertible_v<T, string_view> ) {
        out << "Y\n";
     } else {
         out << "N\n";
     }
}

auto main() -> int
{
    // ostringstream stream;
    // stream << string_view( "Knurr" );
    out << S() << endl;
    //foo( S() );
}
