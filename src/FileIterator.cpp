#include "include/FileIterator.h"

FileIterator::FileIterator(FILE *a) {
    file = a;
    next();
}

char* FileIterator::next() {
    if (!has_next()) {
        return NULL;
    }

    strcpy(cur_line, next_line);

    // Read next line
    // fgets is faster than getLine, hence it's used here
    if (fgets(next_line, DEF_STR_SIZE, file) == NULL) {
        end = true;
    }

    char *e = strpbrk(next_line, "\r\n"); // Replace \r\n with a \0 character
    if (e != NULL) {
        *e = 0;
    }

    if (cur_line[0] == 0) {
        return next();
    }

    return cur_line;
}

char* FileIterator::current() {
    return cur_line;
}

bool FileIterator::has_next() {
    return !end && !closed;
}

void FileIterator::reset() {
    if (closed) {
        return;
    }

    end = false;
    rewind(file);
    next();
}

void FileIterator::close() {
    if (!closed) {
        fclose(file);
        free(cur_line);
        free(next_line);
        closed = true;
    }
}
