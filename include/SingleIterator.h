#ifndef SINGLEITERATOR_H
#define	SINGLEITERATOR_H

#include <string>
#include "Iterator.h"

using namespace std;

class SingleIterator : public Iterator<char*> {
public:

    SingleIterator(char* s);

    char* next();

    char* current();

    bool has_next();

    void reset();

    void close();

private:
    char* a;
    bool gone = false;
};

#endif

