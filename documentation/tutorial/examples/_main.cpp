#define $IO_HW      "io/hello-world.cpp"
#define $IO_FS_DAP  "io/filesystem/display-a-poem.cpp"

#ifndef $WHICH
#   define $WHICH $IO_HW
#endif
#include $WHICH
