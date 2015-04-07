#include <vector>

#include "include/pluto.h"

// Attack from a specific password

void crack() {

    Iterator<char*> *user_it = opts->users;
    Iterator<char*> *pass_it = opts->passwords;

    // Single user & pass, verbose mode
    if (opts->verbose && opts->user_source == USER_GIVEN && opts->pass_source == PASS_GIVEN && !opts->do_mangling) {
        out_combo(verbose_logon(user_it->next(), pass_it->next(), opts->domain));
        outln("Done!");
        return;
    }


    // Setup vars
    vector<string> users;
    bool success = false;
    char* user = MAKE_STR;
    char* pass;
    char* domain = opts->domain;

    // Prepare mangling
    mangle_init();

    // Precache users
    while (user_it->has_next()) {
        users.push_back(string(user_it->next()));
        stat_users++;
    }

    if (users.size() > 1) {
        info("Precached %i usernames", users.size());
    }

    countdown();

    // User loop
    for (string string_user : users) {
        strcpy(user, string_user.c_str());

        // Reset password
        pass_it->reset();

        console_update();

        // Pass loop
        success = false;
        while (pass_it->has_next()) {
            pass = pass_it->next();

            // Check enter
            if (enter_down()) {
                if (opts->enter_info) {
                    outln(string(user) + " > " + string(pass));
                } else {
                    warn("\nStopped.");
                    return;
                }
            }

            // Try logon
            // mangle() handles password filtering
            if (mangle(user, pass, domain)) {
                success = true;
                break; // Next user
            }
        }

        // Out combo
        console_clear();
        out_combo(user, pass, success);

        // Skip check
        if (success && opts->single) {
            break;
        }
    }

    console_clear();
    outln();

    out("Done! ");
    outfln("(%i tries, %i/%i cracked @ %i tps)", stat_tries, stat_cracks, stat_users, stat_tps);

    user_it->close();
    pass_it->close();
}
