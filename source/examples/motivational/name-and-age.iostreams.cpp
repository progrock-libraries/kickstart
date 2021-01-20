#include <iostream>
#include <string>
using   std::cin, std::cout, std::endl,
        std::string, std::stoi;
        
auto fail() -> bool { throw 666; }

auto main() -> int
{
    cout << "Enter your name, please: ";
    string name;
    getline( cin, name ) or fail();
    
    cout << "What's your age, in years? ";
    string age_spec;
    getline( cin, age_spec ) or fail();
    const int age = stoi( age_spec );
    
    cout << "Greetings, " << age << " years old " << name << "!" << endl;
}
