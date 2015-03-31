#include <functional> // std::function
#include <vector> // std::vector
#include "include/pluto.h"
#include "include/rules.h"

using namespace std;

// Rules vector
vector<rule> mangle_rules;

// Size of rules vector for speed
int rules_size;

// Pointers to current data
char *cur_user;
char *cur_pass;
char *cur_domain;

bool do_mangle;

void register_rule(rule r) {
    mangle_rules.push_back(r);
}

void mangle_init() {

    if (!(do_mangle = opts->mangle_pass)) {
        return;
    }

    register_rule(capt);
    register_rule(a123);
    register_rule(a123456);
    register_rule(re3);
    register_rule(ra4);
    rules_size = mangle_rules.size();
}

/*
 * Mangles `cur_pass` with all combinations of `rules_left` amount of rules.
 */
bool rec_mangle(int offset, int rules_left) {
    static char* mangle_pass = MAKE_STR; // Mangled password holder
    static vector<rule> iter_rules; // Rules rec_mangle is currently processing

    if (rules_left == 0) {

        // Prep pass for mangling
        strcpy(mangle_pass, cur_pass);

        // Apply rules
        for (auto rule : iter_rules) {
            rule(mangle_pass, mangle_pass);
        }
        // Clear rules for next mangle
        iter_rules.clear();

        // Filter pass
        if (opts->filter_pass && !filter_pass(mangle_pass)) {
            return false;
        }

        // Try logon
        if (fast_logon(cur_user, mangle_pass, cur_domain)) {
            cur_pass = mangle_pass; // Allow cur_pass to be accessed
            return true;
        }

        return false;
    }

    // Add new rule combo
    for (int i = offset; i <= rules_size - rules_left; ++i) {

        iter_rules.push_back(mangle_rules[i]);
        if (rec_mangle(i + 1, rules_left - 1)) {
            return true;
        }
        iter_rules.pop_back();
    }
    return false;
}

bool mangle(char *user, char *pass, char* domain) {

    // No mangling
    if (!do_mangle) {
        return fast_logon(user, pass, domain);
    }

    // Set password being mangled
    strcpy(cur_pass, pass);

    // Mangle with all sizes of combinations
    for (int i = 1; i < rules_size; i++) {

        // Mangle password with
        if (rec_mangle(0, i)) {

            // Put mangled pass in password pointer
            strcpy(pass, cur_pass);

            return true;
        }

    }

    return false;
}
