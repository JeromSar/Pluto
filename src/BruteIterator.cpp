#include "include/BruteIterator.h"

BruteIterator::BruteIterator(BruteOptions *brute_opts) {
    this->brute_opts = brute_opts;
}

char* BruteIterator::next() {
    if (!has_next()) {
        return NULL;
    }

    // next_pass should have already been populated
    strcpy(cur_pass, next_pass);

    // Calculate next pass
    int i = 0; // Char iterator
    do {
        if (next_pass[i] == chars[chars_len - 1]) { // Iterated char is last char
            carry = true;
            next_pass[i] = chars[0];
        } else {
            carry = false;
            next_pass[i] = *(strchr(chars, next_pass[i]) + 1); // Next char
            break;
        }
    } while (next_pass[++i]);

    // Add new char
    if (carry) {
        carry = false;
        cur_len++;
        next_pass[cur_len - 1] = chars[0];
        next_pass[cur_len] = 0;
    }

    return cur_pass;
}

char* BruteIterator::current() {
    return cur_pass;
}

bool BruteIterator::has_next() {
    return cur_len <= brute_opts->max_len;
}

void BruteIterator::reset() {

    // Populate chars
    char i;
    int j = 0;
    if (brute_opts->lower) {
        for (i = 'a'; i <= 'z'; i++) {
            chars[j++] = i;
        }
    }

    if (brute_opts->upper) {
        for (i = 'A'; i <= 'Z'; i++) {
            chars[j++] = i;
        }
    }


    if (brute_opts->digit) {
        for (i = '0'; i <= '9'; i++) {
            chars[j++] = i;
        }
    }

    if (brute_opts->punct) {
        // http://web.cs.mun.ca/~michael/c/ascii-table.html
        for (i = 0x21; i < 0x7f; i++) {
            if (ispunct(i)) {
                chars[j++] = i;
            }
        }
    }

    if (brute_opts->space) {
        chars[j++] = ' ';
    }

    chars[j] = 0; // End with \0
    chars_len = strlen(chars);


    // Reset next pass
    if (brute_opts->has_start_pass) {
        strcpy(next_pass, brute_opts->start_pass);
        cur_len = strlen(next_pass);
    } else {
        cur_len = brute_opts->min_len;
        for (int i = 0; i < cur_len; i++) {
            next_pass[i] = chars[0];
        }
        next_pass[cur_len] = 0; // \0 terminator
    }
    carry = false;
}

void BruteIterator::close() {
    free(chars);
    free(cur_pass);
}


