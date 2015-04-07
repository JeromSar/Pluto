#ifndef BASE_H
#define	BASE_H

#include <cstdlib> // malloc
#include <cstdio> // vfprintf
#include <cstdarg> // va_list
#include <iostream> // cout

// Base header file on which all other header files can depend

using namespace std;

// Project constants
#define NAME std::string("Pluto")
#define VERSION std::string("2.2.0")
#define AUTHOR std::string("Overlord")

// Other constants
#define DEF_STR_SIZE 256
#define MAKE_STR (char*) malloc(DEF_STR_SIZE)
#define MAKE_STR_100 (char*) malloc(100)

// stat.cpp
extern int stat_users;
extern int stat_tries;
extern int stat_cracks;

// Console utilities

inline void out(string str) {
    cout << str;
}

inline void outln(string str) {
    cout << str << endl;
}

inline void outln() {
    cout << endl;
}

inline void outfln(const char* format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stdout, format, argptr);
    outln();
    va_end(argptr);
}

inline void info(string str) {
    outln("[*] " + str);
}

inline void info(const char* format, ...) {
    va_list argptr;
    va_start(argptr, format);
    out("[*] ");
    vfprintf(stdout, format, argptr);
    outln();
    va_end(argptr);
}

inline void fine(string str) {
    outln("[+] " + str);
}

inline void warn(string str) {
    outln("[!] " + str);
}


#endif
