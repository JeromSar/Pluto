#include "include/FileIterator.h"

FileIterator::FileIterator(FILE *a) {
    file = a;
    next();
}

char* FileIterator::next() {
    if (!has_next()) {
        return NULL;
    }

    // Prepare current line
    strcpy(cur_line, next_line);

    // Get next line
    next_line[0] = 0;
    while (next_line[0] == 0) {
        // fgets is faster than getLine, hence it's used here
        if (fgets(next_line, DEF_STR_SIZE, file) == NULL) {
            end = true;
            break;
        }

        char *e = strpbrk(next_line, "\r\n"); // Replace \r\n with a \0 character
        if (e != NULL) {
            *e = 0;
        }
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
        closed = true;
        fclose(file);
        free(cur_line);
        free(next_line);
    }
}
