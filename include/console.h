#ifndef CONSOLE_H
#define	CONSOLE_H
#include <cstdio> // vfprintf
#include <cstdarg> // va_list
#include <iostream> // cout

using namespace std;

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

#endif
