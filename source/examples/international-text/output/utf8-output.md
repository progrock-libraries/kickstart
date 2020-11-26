# Display international text in the console.

Displaying international text in a portable manner is easy with Kickstart:

[*examples/international-text/output/international-text.kickstart.cpp*]:
~~~cpp
// Source encoding: utf-8  --  π is (or should be) a lowercase Greek pi.
#include <kickstart/core.hpp>
using namespace kickstart::core;

auto main() -> int
{
    output( "Every 日本国 кошка loves Norwegian “blåbærsyltetøy”!\n" );
}
~~~

To the best of my knowledge “日本国 кошка” means “Japanese cat”, with “Japanese” written in Chinese and “cat” written in Russian. “Blåbærsyltetøy” is Norwegian for blueberry jam. Thus the text says, in a mixture of languages, that every Japanese cat loves Norwegian blueberry jam.

## 1. Windows console limitations compared to Linux consoles.



