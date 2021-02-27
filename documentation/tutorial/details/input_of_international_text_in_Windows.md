# Detail — <br>**Input of international text in Windows.**

Here’s an attempt to input international text using the C++ standard library directly:

*File ([io/personalized-kickstart-greeting.stdlib.cpp](examples/io/personalized-kickstart-greeting.stdlib.cpp)):*
~~~cpp
#include <iostream>
#include <string>
using namespace std;

auto fail() -> bool { throw "Gah!"; }

auto main() -> int
{
    string name;
    cout << "Hi, what’s your name? ";
    getline( cin, name ) or fail();
    cout << "Welcome to the C++ standard library experience, " << name << "!" << endl;
}
~~~

Typical result in a default Windows classic console on the author’s system, with a `chcp` command reporting the active codepage:

> Active code page: 850  
> Hi, whatÔÇÖs your name? ***Bjørn Håvard Sæther***  
> Welcome to the C++ standard library experience, Bjørn Håvard Sæther!

Uhuh. Some trouble with the UTF-8 encoded round apostrophe, so let’s switch to codepage 65001:

> Active code page: 65001  
> Hi, what’s your name? ***Bjørn Håvard Sæther***  
> Welcome to the C++ standard library experience, Bj rn H vard S ther!

Now the round apostrophe is correctly presented. Codepage 65001 works for standard library UTF-8 *output*. But the non-ASCII characters in the name “Bjørn Håvard Sæther” are not handled correctly: they end up being presented as spaces!

Actually the Windows API level’s `ReadFile` function produces a string with zero values for these input characters, and the current Windows implementations of the C++ standard library all blithely assume that `ReadFile` works, so codepage 65001 decidedly does not work for standard library *input*.

And as of early 2021 this happens with C++ standard library input whether one uses a classic console or Microsoft Terminal.
