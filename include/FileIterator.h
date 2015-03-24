#ifndef FILEITERATOR_H
#define	FILEITERATOR_H

#include <cstdio>
#include <string>
#include <cstring> // strpbrk()
#include "console.h"
#include "constants.h"
#include "Iterator.h"

using namespace std;

class FileIterator : public Iterator<char*> {
public:

    FileIterator(FILE *a);

    char* next();

    char* current();

    bool has_next();

    void reset();

    void close();

private:
    FILE *file;
    bool closed = false;
    bool end = false;
    char* cur_line = MAKE_STR;
    char* next_line = MAKE_STR;
};

#endif

