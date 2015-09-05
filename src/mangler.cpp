#include <functional> // std::function
#include <vector> // std::vector
#include <algorithm>  // std::sort
#include "include/pluto.h"
#include "include/rules.h"

using namespace std;

vector<rule> mangle_rules;
vector<string> prefixes;
vector<string> suffixes;


int mangle_depth; // Amount of mangle_rules to apply combinatorally
bool do_any_mangle;
bool do_std_mangle;
bool do_prefix;
bool do_suffix;

char *cur_pass = MAKE_STR;
vector<string> cur_mangles;

/*
 * Clears and repopulates 'mangle_rules' based on the 'opts' variable.
 */
void mangle_init() {
    static bool init = false;
    if (init) {
        return;
    }
    init = true;

    if (!(do_any_mangle = opts->do_mangling)) {
        return;
    }

    // Prefixes
    prefixes.clear();
    if ((do_prefix = opts->mangle_opts->prefix)) {
        prefixes.push_back("123");
        prefixes.push_back("hello");
        info("Loaded %i prefixes", prefixes.size());
    }

    // Standard mangling
    mangle_rules.clear();
    do_std_mangle = opts->mangle_opts->leetspeak || opts->mangle_opts->capt;
    if (do_std_mangle) {

        // L33tspeak
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

        // Capitalization
        if (opts->mangle_opts->capt) {
            register_mangle_rule(capt_first_letter);
            register_mangle_rule(capt_all);
        }

        // Mangle depth
        if ((mangle_depth = opts->mangle_opts->depth) == 0) {
            mangle_depth = mangle_rules.size();
        }
        info("Loaded %i mangling rules (depth: %i)", mangle_rules.size(), mangle_depth);
    }

    // Suffixes
    suffixes.clear();
    if ((do_suffix = opts->mangle_opts->suffix)) {
        suffixes.push_back("!");
        suffixes.push_back("?");
        suffixes.push_back("0");
        suffixes.push_back("001");
        suffixes.push_back("007"); // James bond :P

        // 1-9 & 01-09
        for (int i = 1; i <= 9; i++) {
            suffixes.push_back(itostr(i));
            suffixes.push_back(string("0") + itostr(i));
        }

        // 1995-2016
        for (int i = 1995; i <= 2015; i++) {
            suffixes.push_back(itostr(i));
        }

        // 1-12345678
        for (int i = 1; i <= 8; i++) {
            string str = string();
            for (int j = 1; j <= i; j++) {
                str += string(itostr(j));
            }
            suffixes.push_back(str);
        }

        // Remove dupes
        sort(suffixes.begin(), suffixes.end());
        suffixes.erase(unique(suffixes.begin(), suffixes.end()), suffixes.end());

        info("Loaded %i suffixes", suffixes.size());
    }
}

/**
 * Populates 'cur_mangles' with 'rules_left' combinations of mangle rules.
 * Uses 'cur_pass' as the base password.
 */
void pop_mangle_combos(int offset, int rules_left) {
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
        pop_mangle_combos(i + 1, rules_left - 1);
        iter_rules.pop_back();
    }
}

/*
 * Populates 'cur_mangles' with 'cur_pass' as the base password.
 */
void pop_mangle_list() {
    cur_mangles.push_back(string(cur_pass));

    // Mangle password: populates `cur_mangles`
    if (mangle_rules.size() > 0) {
        for (int i = 1; i <= mangle_depth; i++) {
            // Mangle password with i amount of rules
            pop_mangle_combos(0, i);
        }
    }

    // Populate prefixes, suffixes
    vector<string> prefsuf_mangles;
    if (do_prefix) {
        for (string prefix : prefixes) {
            for (string mangle : cur_mangles) {
                prefsuf_mangles.push_back(prefix + mangle);
            }
        }
        prefsuf_mangles.push_back(string(cur_pass) + string(cur_pass));
    }
    if (do_suffix) {
        for (string suffix : suffixes) {
            for (string mangle : cur_mangles) {
                prefsuf_mangles.push_back(mangle + suffix);
            }
        }
        prefsuf_mangles.push_back(string(cur_pass) + string(cur_pass));
    }
    if (do_prefix && do_suffix) {
        for (string prefix : prefixes) {
            for (string suffix : suffixes) {
                for (string mangle : cur_mangles) {
                    prefsuf_mangles.push_back(prefix + mangle + suffix);
                }
            }
        }
    }

    // Add prefixes, suffixes
    // http://stackoverflow.com/questions/3177241/best-way-to-concatenate-two-vectors
    cur_mangles.reserve(prefsuf_mangles.size()); // preallocate memory
    cur_mangles.insert(cur_mangles.end(), prefsuf_mangles.begin(), prefsuf_mangles.end());
}

/**
 * Mangles the password and tries all user/pass combos.
 */
vector<string>* mangle(const char *pass) {
    cur_mangles.clear();

    // No mangling?
    if (!opts->do_mangling) {
        cur_mangles.push_back(pass);
        return &cur_mangles;
    }

    // Store base pass in 'cur_pass'
    strcpy(cur_pass, pass);

    // Populate mangle list and update mangles/user
    pop_mangle_list();

    // Remove duplicates
    // http://stackoverflow.com/questions/5487197/check-for-duplicates-in-large-vector-of-strings
    sort(cur_mangles.begin(), cur_mangles.end());
    cur_mangles.erase(unique(cur_mangles.begin(), cur_mangles.end()), cur_mangles.end());

    update_mpu(cur_mangles.size());

    return &cur_mangles;
}
