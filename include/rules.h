#ifndef RULES_H
#define	RULES_H

#include "base.h"

rule capt_first_letter = [ = ](char *base, const char *pass){
    strcpy(base, pass);
    base[0] = toupper(base[0]);
    return base;
};

rule capt_all = [ = ](char *base, const char *pass){
    strcpy(base, pass);
    int i;
    for (i = 0; base[i]; i++) {
        base[i] = toupper(base[i]);
    }
    //terminate string
    base[i] = '\0';
};

extern vector<rule> mangle_rules;

inline void register_rule(rule r) {
    mangle_rules.push_back(r);
}

inline void register_rep_rule(const char *from, const char *to) {
    register_rule([ = ](char *base, const char *pass){
        strcpy(base, pass);
        strrep(base, from, to);
        return base;
    });
}

inline void register_suf_rule(const char *append) {
    register_rule([ = ](char *base, const char *pass){
        strcpy(base, pass);
        strcat(base, append);
        return base;
    });
}

#endif

