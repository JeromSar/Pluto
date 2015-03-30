#include "include/SingleIterator.h"

SingleIterator::SingleIterator(char* s) {
    a = s;
}

char* SingleIterator::next() {
    if (!has_next()) {
        return NULL;
    }
    gone = true;
    return a;
}

char* SingleIterator::current() {
    if (!gone) { // pointer before str
        return NULL;
    }

    return a;
}

bool SingleIterator::has_next() {
    return !gone;
}

void SingleIterator::reset() {
    gone = false;
}

void SingleIterator::close() {
}


