#include "include/pluto.h"

// Attack from a specific password

void crack() {

    // Prepare mangling
    mangle_init();

    Iterator<char*> *user_it = opts->users;
    Iterator<char*> *pass_it = opts->passwords;

    // Single user & pass, verbose mode
    if (opts->verbose && opts->user_source == USER_GIVEN && opts->pass_source == PASS_GIVEN && !opts->do_mangling) {
        out_combo(verbose_logon(user_it->next(), pass_it->next(), opts->domain));
        outln("Done!");
        return;
    }

    // Setup vars
    bool success = false;
    char* user;
    char* pass;
    char* domain = opts->domain;

    countdown();

    // User loop
    while (user_it->has_next()) {
        user = user_it->next();
        stat_users++;

        // Reset password
        pass_it->reset();

        // Pass loop
        success = false;
        while (pass_it->has_next()) {
            pass = pass_it->next();

            // Check enter
            if (enter_down()) {
                if (opts->enter_info) {
                    outln(string(user) + " > " + string(pass));
                } else {
                    outln("\nStopped.");
                    return;
                }
            }

            // New try
            if (opts->limit_tries && stat_tries > opts->max_tries) {
                break;
            }


            // Try logon
            // mangle() handles password filtering
            if (mangle(user, pass, domain)) {
                success = true;
                stat_cracks++;
                break; // Next user
            }
        }

        // Out combo
        out_combo(user, pass, success);

        // Skip check
        if (success && opts->single) {
            break;
        }
    }

    outln();
    if (user_it->has_next()) {
        out("Stopped. ");
    } else {
        out("Done! ");
    }
    outfln("(%i tries, %i/%i cracked)", stat_tries, stat_cracks, stat_users);

    user_it->close();
    pass_it->close();
}
