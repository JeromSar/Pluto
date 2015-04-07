#include <functional> // std::function
#include <vector> // std::vector
#include <algorithm>  // std::sort
#include "include/pluto.h"
#include "include/rules.h"

using namespace std;

vector<rule> mangle_rules;
vector<string> prefixes;
vector<string> suffixes;

bool do_any_mangle;
bool do_suffix;
bool do_mangle;
bool do_prefix;

int mangle_depth; // Amount of mangle_rules to apply combinatorally

// Pointers to current data, only cur_pass is copied
char *cur_user;
char *cur_pass = MAKE_STR;
char *cur_domain;
vector<string> cur_mangles;

void mangle_init() {

    if (!(do_any_mangle = opts->do_mangling)) {
        return;
    }

    do_prefix = opts->mangle_opts->prefix;
    do_mangle = opts->mangle_opts->leetspeak || opts->mangle_opts->capt;
    do_suffix = opts->mangle_opts->suffix;
    mangle_depth = opts->mangle_opts->depth;

    //
    // Prefix
    //
    prefixes.push_back("123");

    if (do_prefix) {
        info("Loaded %i prefixes", prefixes.size());
    }

    //
    // Mangle
    //
    if (opts->mangle_opts->leetspeak) {
        register_mangle_rule("l", "1");
        register_mangle_rule("a", "4");
        register_mangle_rule("a", "@");
        register_mangle_rule("e", "3");
        register_mangle_rule("t", "7");
        register_mangle_rule("o", "0");
        register_mangle_rule("s", "5");
        register_mangle_rule("i", "1");
        register_mangle_rule("s", "$");
    }
    if (opts->mangle_opts->capt) {
        register_mangle_rule(capt_first_letter);
        register_mangle_rule(capt_all);
    }

    if (mangle_depth == 0) {
        mangle_depth = mangle_rules.size();
    }

    if (do_mangle) {
        info("Loaded %i mangling rules (depth: %i)", mangle_rules.size(), mangle_depth);
    }

    //
    // Suffixes
    //
    suffixes.push_back("!");
    suffixes.push_back("?");
    suffixes.push_back("123");

    // 1-9 & 01-09
    for (int i = 1; i <= 9; i++) {
        suffixes.push_back(itostr(i));
        suffixes.push_back(string(string("0") + string(itostr(i))).c_str());
    }

    suffixes.push_back("0");
    suffixes.push_back("001");
    suffixes.push_back("007"); // James bond :P

    // 1995-2016
    for (int i = 1995; i <= 2016; i++) {
        suffixes.push_back(itostr(i));
    }

    // 1-12345678
    for (int i = 1; i <= 8; i++) {
        string str;
        for (int j = 1; j <= i; j++) {
            str += string(itostr(j));
        }
        suffixes.push_back(str.c_str());
    }

    // Remove duplicates
    sort(suffixes.begin(), suffixes.end());
    suffixes.erase(unique(suffixes.begin(), suffixes.end()), suffixes.end());

    if (do_suffix) {
        info("Loaded %i suffixes", suffixes.size());
    }
}

/*
 * Mangles `cur_pass` with all combinations of `rules_left` amount of rules.
 * Result is stored in `cur_mangles`.
 */
void rec_mangle(int offset, int rules_left) {
    static char* mangle_pass = MAKE_STR; // Mangled password holder
    static vector<rule> iter_rules; // Rules rec_mangle is currently processing

    if (offset == 0) {
        iter_rules.clear();
    }

    if (rules_left == 0) {
        strcpy(mangle_pass, cur_pass);

        // Apply rules
        for (rule rule : iter_rules) {
            rule(mangle_pass, mangle_pass);
        }

        cur_mangles.push_back(string(mangle_pass));
        return;
    }

    // Recurse with new rule combo
    for (uint32_t i = offset; i <= mangle_rules.size() - rules_left; ++i) {
        iter_rules.push_back(mangle_rules[i]);
        rec_mangle(i + 1, rules_left - 1);
        iter_rules.pop_back();
    }
}

// TODO proper docs?

/**
 * Mangles the password and tries all user/pass combos.
 * Returns true if one of the combinations is correct.
 * In such a case, the correct password is stored in `pass`.
 */
bool mangle(char *user, char *pass, char* domain) {

    // Check no mangling
    if (!do_any_mangle) {

        // Filter
        if (opts->filter_pass && filter_pass(pass)) {
            if (opts->verbose) {
                outln("f " + string(user) + " | " + string(pass));
            }
            return false;
        }

        if (opts->verbose) {
            outln("> " + string(user) + " | " + string(pass));
        }

        return fast_logon(user, pass, domain);
    }

    // Clear previous mangles
    int mangle_tries = 0;
    cur_mangles.clear();

    // Set password being mangled
    strcpy(cur_pass, pass);

    // Mangling start //

    cur_mangles.push_back(string(cur_pass));

    // Mangle password: populates `cur_mangles`
    if (do_mangle) {
        for (int i = 1; i <= mangle_depth; i++) { //
            // Mangle password with i amount of rules
            rec_mangle(0, i);
        }
    }

    // Remove duplicates
    // http://stackoverflow.com/questions/5487197/check-for-duplicates-in-large-vector-of-strings
    sort(cur_mangles.begin(), cur_mangles.end());
    cur_mangles.erase(unique(cur_mangles.begin(), cur_mangles.end()), cur_mangles.end());


    // Populate prefixes, suffixes
    vector<string> new_mangles;
    if (do_prefix) {
        for (string prefix : prefixes) {
            for (string mangle : cur_mangles) {
                new_mangles.push_back(prefix + mangle);
            }
        }
        new_mangles.push_back(string(cur_pass) + string(cur_pass));
    }
    if (do_suffix) {
        for (string suffix : suffixes) {
            for (string mangle : cur_mangles) {
                new_mangles.push_back(mangle + suffix);
            }
        }
    }
    if (do_prefix && do_suffix) {
        for (string prefix : prefixes) {
            for (string suffix : suffixes) {
                for (string mangle : cur_mangles) {
                    new_mangles.push_back(prefix + mangle + suffix);
                }
            }
        }
    }

    // Add prefixes, suffixes
    // http://stackoverflow.com/questions/3177241/best-way-to-concatenate-two-vectors
    cur_mangles.reserve(new_mangles.size()); // preallocate memory
    cur_mangles.insert(cur_mangles.end(), new_mangles.begin(), new_mangles.end());

    update_mpu(cur_mangles.size());
    // Mangling end //


    int start_millis = ms_timestamp();
    for (string mangle : cur_mangles) {

        // Check mangle
        if (opts->filter_pass && !filter_pass(mangle.c_str())) {
            if (opts->verbose) {
                outln("f " + string(user) + " | " + mangle);
            }
            continue;
        }

        mangle_tries++;
        if (opts->max_tries != 0 && mangle_tries > opts->max_tries) {
            return false;
        }

        // Store in cur_pass since we're not using that anymore anyway
        strcpy(cur_pass, mangle.c_str());

        if (opts->verbose) {
            outln("> " + string(user) + " | " + string(cur_pass));
        }

        // Logon
        if (fast_logon(user, cur_pass, domain)) {
            strcpy(pass, cur_pass); // Copy password to the original pointer
            update_tps(mangle_tries, ms_timestamp() - start_millis);
            return true;
        }

    }
    update_tps(mangle_tries, ms_timestamp() - start_millis);

    return false;
}
