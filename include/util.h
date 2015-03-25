#ifndef UTIL_H
#define UTIL_H

#include <conio.h> // _getch(), _kbhit()
#include <string> // std::string
#include "constants.h" // MAKE_STR
#include "console.h"
#include "Iterator.h"
#include "FileWriter.h"

using namespace std;

enum UserSource {
    USER_FILE,
    USER_GIVEN,
};

enum PassSource {
    PASS_FILE,
    PASS_GIVEN,
    PASS_BRUTEFORCE,
};

struct FilterOptions {
    int min_alpha = 0;
    int min_lower_alpha = 0;
    int min_upper_alpha = 0;
    int min_digit = 0;
    int min_punct = 0;
    int min_digit_or_punct = 0;
    int min_size = 0;
};

struct BruteOptions {
    int min_len = 3;
    int max_len = 6;
    bool lower = false;
    bool upper = false;
    bool digit = false;
    bool punct = false;
    bool space = false;
    bool has_start_pass = false;
    char* start_pass = MAKE_STR;
};

struct Options {
    bool verbose = false;
    bool help = false;
    bool interactive = false;
    bool single = false; // Stop if the first crack has been found
    bool quiet = false; // Minimal output
    bool enter_info = false;

    char *domain = MAKE_STR;

    bool limit_tries = false;
    int max_tries = 0;

    UserSource user_source;
    Iterator<char*> *users;

    PassSource pass_source;
    Iterator<char*> *passwords;
    BruteOptions *brute_opts = new BruteOptions;

    bool filter_pass = false;
    FilterOptions *filter_opts = new FilterOptions;

    bool write_out_file = false;
    FileWriter *out_file;

    Options() {
        domain[0] = '.';
        domain[1] = '\0';
    }
};

inline bool enter_down() {
    if (_kbhit() && _getch() == '\r') {
        return true;
    }
    return false;
}



#endif
