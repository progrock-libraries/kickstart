# Detail — <br>**Output of international text in Windows.**

Using the C++ standard library directly to output some international text:

*File ([io/bhs.stdlib.cpp](examples/io/bhs.stdlib.cpp)):*
~~~cpp
#include <iostream>
using namespace std;

auto main() -> int
{
    cout << "Dear world, Bjørn Håvard Sæther says hello!" << endl;
}
~~~

Result in a Windows console, using the Visual C++ compiler with `/utf8` option and the defaults on the author’s system:

> Dear world, Bj├©rn H├Ñvard S├ªther says hello!

The gobbledygook appears because the console expects a different text encoding than UTF-8, and because *the Visual C++ standard library implementation does not address that issue*.

---

This is so also with the other common Windows compilers. The Windows C++ standard library implementations could have done the same as Kickstart, with Just Works&trade; behavior, but they don’t. Presumably that common misbehavior is due to common reuse of a core of flawed functionality in Microsoft’s C runtime library.

---

When you work in a Windows command interpreter, as opposed to running your program from an IDE or by double-clicking the executable, then you can use the `chcp` command to tell the console to expect UTF-8 encoding:

> [C:\my\dev\libraries\kickstart-1\documentation\tutorial\examples\io]  
> \> ***cl bhs.stdlib.cpp /Fe"bhs-stdlib"***  
> bhs.stdlib.cpp  
>  
> [C:\my\dev\libraries\kickstart-1\documentation\tutorial\examples\io]  
> \> ***chcp & bhs-stdlib***  
> Active code page: 850  
> Dear world, Bj├©rn H├Ñvard S├ªther says hello!
> 
> [C:\my\dev\libraries\kickstart-1\documentation\tutorial\examples\io]  
> \> ***chcp 65001 & bhs-stdlib***  
> Active code page: 65001  
> Dear world, Bjørn Håvard Sæther says hello!

`chcp` is short for *change codepage*. A **codepage** is a number that identifies a text encoding &mdash; it’s a scheme originally used by IBM, and adopted and adapted by Microsoft, SAS and others. Windows codepage 850 identifies a Scandinavian variant of the original IBM PC text encoding, and Windows codepage 65001 identifies UTF-8.

As of 2021, in Windows 10 codepage 65001 works for standard library UTF-8 *output*, as shown above, and with [Microsoft Terminal](https://github.com/microsoft/terminal) it even works for emojis 😃, but it doesn’t work for standard library *input* 😒.

