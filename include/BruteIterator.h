#ifndef BRUTEITERATOR_H
#define	BRUTEITERATOR_H

#include "Iterator.h"
#include "util.h" // BruteOptions
#include <cstdlib> // free())
#include <cstring> // strcpy())
#include <cctype> // ispunct(), isalpha())


using namespace std;

class BruteIterator : public Iterator<char*> {
public:

    BruteIterator(BruteOptions *brute_opts);

    char* next();

    char* current();

    bool has_next();

    void reset();

    void close();

private:
    BruteOptions *brute_opts;
    int cur_len = 0;
    bool carry = false;
    char* chars = MAKE_STR;
    int chars_len = 0;
    char* cur_pass = MAKE_STR;
    char* next_pass = MAKE_STR;
};

#endif
