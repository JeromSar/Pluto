#include "include/pluto.h"

int i = 0;
int pass_size = 0;
char c;
//
int alpha = 0;
int lower_alpha = 0;
int upper_alpha = 0;
int digit = 0;
int punct = 0;
int digit_or_punct = 0;

bool filter_pass(char* pass) {

    pass_size = strlen(pass);

    if (pass_size < opts->filter_opts->min_size) {
        return false;
    }

    alpha = 0;
    lower_alpha = 0;
    upper_alpha = 0;
    digit = 0;
    punct = 0;
    digit_or_punct = 0;
    for (i = 0; i < pass_size; i++) {
        c = pass[i];

        if (isdigit(c)) {
            digit++;
            digit_or_punct++;
            continue;
        }

        if (isalpha(c)) {
            alpha++;

            if (c >= 'a' && c <= 'z') {
                lower_alpha++;
            } else {
                upper_alpha++;
            }
            continue;
        }

        if (ispunct(c) || c == ' ') {
            punct++;
            digit_or_punct++;
            continue;
        }
    }

    if (alpha < opts->filter_opts->min_alpha
            || lower_alpha < opts->filter_opts->min_lower_alpha
            || upper_alpha < opts->filter_opts->min_upper_alpha
            || digit < opts->filter_opts->min_digit
            || punct < opts->filter_opts->min_punct
            || digit_or_punct < opts->filter_opts->min_digit_or_punct) {
        return false;
    }

    return true;
}

