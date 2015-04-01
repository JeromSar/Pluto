#include <functional> // std::function
#include <vector> // std::vector
#include <algorithm>  // std::sort
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
vector<string> cur_mangles;

bool do_mangle;

void mangle_init() {

    if (!(do_mangle = opts->mangle_pass)) {
        return;
    }

    //
    // Replacements
    //

    // Replace-all
    register_rep_rule("l", "1");
    register_rep_rule("a", "4");
    register_rep_rule("a", "@");
    register_rep_rule("e", "3");

    //
    // Suffixes
    //

    // Special characters
    register_suf_rule("!");
    register_suf_rule("?");

    // 0-15
    for (int i = 0; i <= 15; i++) {
        register_suf_rule(itostr(i));
        register_suf_rule(string(string("00") + string(itostr(i))).c_str());
    }

    // 1995-2018
    for (int i = 1995; i <= 2018; i++) {
        register_suf_rule(itostr(i));
    }

    // 1 - 12345678
    for (int i = 1; i <= 8; i++) {
        string str;
        for (int j = i; j <= i; j++) {
            str += itostr(j);
        }
        register_suf_rule(str.c_str());
    }


    // Other rules
    register_rule(capt_first_letter);
    register_rule(capt_all);

    rules_size = mangle_rules.size();
}

/*
 * Mangles `cur_pass` with all combinations of `rules_left` amount of rules.
 * Result is stored in `cur_mangles`.
 */
void rec_mangle(int offset, int rules_left) {
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
            return;
        }

        cur_mangles.push_back(string(mangle_pass));
        return;
    }

    // Recurse with new rule combo
    for (int i = offset; i <= rules_size - rules_left; ++i) {
        iter_rules.push_back(mangle_rules[i]);
        rec_mangle(i + 1, rules_left - 1);
        iter_rules.pop_back();
    }
}

bool mangle(char *user, char *pass, char* domain) {

    // Check no mangling
    if (!do_mangle) {

        // Filter
        if (filter_pass(pass)) {
            return false;
        }

        return fast_logon(user, pass, domain);
    }

    // Clear previous mangles
    cur_mangles.clear();

    // Set password being mangled
    strcpy(cur_pass, pass);

    // Mangle password: populates `cur_mangles`
    for (int i = 1; i < rules_size; i++) {
        // Mangle password with i amount of rules
        rec_mangle(0, i);
    }

    // Remove duplicates
    // http://stackoverflow.com/questions/5487197/check-for-duplicates-in-large-vector-of-strings
    sort(cur_mangles.begin(), cur_mangles.end());
    cur_mangles.erase(unique(cur_mangles.begin(), cur_mangles.end()), cur_mangles.end());

    for (string mangle : cur_mangles) {

        // Check mangle
        if (filter_pass(mangle.c_str())) {
            continue;
        }

        // Store in cur_pass since we're not using that anymore anyway
        strcpy(cur_pass, mangle.c_str());

        // Logon
        if (fast_logon(user, cur_pass, domain)) {
            // Copy mangled password in the original pointer
            strcpy(pass, cur_pass);
            return true;
        }
    }

    return false;
}
