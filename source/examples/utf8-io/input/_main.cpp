#define $P1 "personalized-greeting.minimal.cpp"
#define $P2 "personalized-greeting.solid.cpp"

#ifndef $WHICH
#   define $WHICH $P2
#endif
#include $WHICH
