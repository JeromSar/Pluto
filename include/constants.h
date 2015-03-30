#ifndef CONSTANTS_H
#define	CONSTANTS_H

#include <cstdlib> // malloc

using namespace std;

#define NAME std::string("Pluto")
#define VERSION std::string("2.2.0")
#define AUTHOR std::string("Overlord")

// strings
#define DEF_STR_SIZE 256
#define MAKE_STR (char*) malloc(DEF_STR_SIZE)
#define MAKE_STR_100 (char*) malloc(100)

#endif

