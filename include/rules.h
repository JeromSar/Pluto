#ifndef RULES_H
#define	RULES_H

#include "base.h"

rule capt = [ = ](char *base, const char *pass){
    strcpy(base, pass);
    base[0] = toupper(base[0]);
    return base;
};

rule a123 = [ = ](char *base, const char *pass){
    strcpy(base, pass);
    strcat(base, "123");
    return base;
};

rule a123456 = [ = ](char *base, const char *pass){
    strcpy(base, pass);
    strcat(base, "123456");
    return base;
};

rule re3 = [ = ](char *base, const char *pass){
    strcpy(base, pass);
    strrep(base, "e", "3");
    return base;
};

rule ra4 = [ = ](char *base, const char *pass){
    strcpy(base, pass);
    strrep(base, "a", "4");
    return base;
};


#endif

