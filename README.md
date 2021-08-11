# **Kickstart**

Small exercises should be as simple in C++ as in Python.

Well that's impossible but this **C++ 17 header library** can help.

There’s a separate “Hello, world!”-document with [detailed installation instructions](documentation/installation/main.md), and there’s also a Work In Progress&trade; [tutorial](documentation/tutorial/main.md).

### ***Thanks:***
– Jon Macey [noted](https://www.facebook.com/groups/cppInPracticeQuestions/permalink/3863055490382079/?comment_id=3863694366984858&reply_comment_id=3863759706978324) that clang requires “`= 0`” for the [`bits_per_mantissa_` declaration](source/library/kickstart/core/stdlib-extensions/limits.hpp#L36), plus some more.<br>
– Yahya Mohammed [noted](https://www.facebook.com/groups/cppInPracticeQuestions/permalink/3894807053873589/?comment_id=3896282353726059) a [missing `#include`](source/library/kickstart/root/system-specific/linux/get_commandline_data.impl.hpp#L35) directive.<br>
– Max Yttril [noted](https://www.facebook.com/groups/cppInPracticeQuestions/permalink/3949130701774557/?comment_id=3965718920115735&__cft__[0]=AZVzQCehbjFgUJyVy5GuzCrOfEnAZOJyH4zmDV8M-ZIQqOGrLahR976YaZnht5bxncmid-XIF0XrtMFJTQ9H-lXzzAP3dy-LqT152l7WDQnEnj2FuGcb_FFcb-Di3ZmuwkDtnkUri6GvCW5dIHxXmjdw&__tn__=R]-R) some problems with compiling Kickstart on the Mac.
## ***Example usage:***

~~~cpp
#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main() -> int
{
    const string    name    = input( "Enter your name, please: " );
    const int       age     = to_<int>( input( "What’s your age, in years? " ) );
    
    out << "Greetings, " << age << " years old " << name << "!" << endl;
}
~~~

In [the above code](source/examples/motivational/name-and-age.kickstart.cpp):

* The `kickstart::all` namespace here provides
  - `input`, a Python-like UTF-8 input function,
  - `to_<int>`, a conversion function, and
  - `out` and `endl`, output functionality that corresponds roughly to the standard library’s `std::cout` and `std::endl`, but that supports UTF-8 also in Windows.

* **`input`** inputs a line of text as an UTF-8 encoded string.  
  In particular this means that
  - the user can type in non-English characters such as Norwegian ‘æ’, ‘ø’ and ‘å’, and that
  - the name that's input can contain spaces.

* **`to_<int>`** converts a string to `int`, or *fails with an exception*.  
  I.e. you never get an arbitrary result on failure, as iostreams give you by default.

* **`out <<`** converts the arguments to text and outputs that text, with UTF-8 interpretation also in a Windows console.

* The **`using namespace kickstart::all;`** directive is in practice safe.  
  The unfortunately common `using namespace std;` directive can be a dangerous habit since one risks name collisions. The `using namespace kickstart::all` directive is much more safe.
  
## ***Some rationales.***

***Q.*** Why is input done as the expression result of a function call `input()` instead of the modification of variables that one gets with iostreams `>>` or `std::getline`?

***A.*** The input-as-expression-result supports use of `const`, and that in turn makes the code shorter and more clear.

---

***Q.*** Why are iostreams headers not included by default?

***A.*** Bcause there’s a possible cost of increased size of the executable. Kickstart input and output is instead by default implemented in terms of C `stdin`, `stdout` and `stderr` operations.

---

***Q***. What’s this about UTF-8 support?

***A***. Windows consoles do not by default support the UTF-8 encoding that’s necessary for portable input and output of international text, and Windows implementations of the standard library unfortunately do not compensate for that. The standard library implementations just use the OS’s byte stream functionality directly. But when input comes directly from a Windows console, or output goes directly to a Windows console, then Kickstart instead uses the UTF-16 based Windows API so as to deal correctly with non-ASCII characters, with proper conversion to/from UTF-8.

## ***Some technical details:***

* The `to_<int>` function ignores ASCII whitespace on the ends of the string, but other extraneous characters cause failure.

* The `out` stream also supports [ANSI escape sequences](source/examples/console/colored-text.cpp#L9-L13) e.g. for [**colors**](source/library/kickstart/root/console/ansi_escape_seq.hpp#L102-L149).
 
* The `kickstart::all` namespace also provides [a few select standard library identifiers](source/library/kickstart/core/namespace.language/stdlib-includes/basics.hpp#L37-L55). In particular you can use `string`, `array` and `vector` directly instead of writing `std::string`, `std::array` and `std::vector`.

If `to_<int>` ignored all characters after a valid specification then text input `3,14` would erroneously convert to number `3`, and the program could then produce an incorrect but still plausible result. That’s unfortunately the default in the standard library, but Kickstart fixes that for you. If `to_<int>` finds extraneous characters other than leading or trailing whitespace, then it fails with an exception.

The library offers a hierarchy of functionality so that you can choose a suitable level of abstraction and detail for the task at hand. For example, `to_<int>` is just a thin checking wrapper around `to_<double>`, which in turn is effectively an alias for `safe::trimmed_string_to_double`. And for example, `out << v` effectively invokes just `output(str(v))`.
