#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main() -> int
{
    const string    name    = input( "Enter your name, please: " );
    const int       age     = to_<int>( input( "What's your age, in years? " ) );
    
    out << "Greetings, " << age << " years old " << name << "!" << endl;
}
