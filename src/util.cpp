#include  "include/pluto.h"

#include <iostream> // fopen()
#include <sstream> // stringstream
#include <string> // std::to_string

// Some display utilities

void verbose_combo(const char* user, const char* pass, bool filtered) {
    if (!opts->verbose) {
        return;
    }

    if (filtered) {
        info("f " + string(user) + " | " + string(pass));
    } else {
        info("x " + string(user) + " | " + string(pass));
    }
}

void out_combo(string user) {
    fine(user + ":UNKNOWN");
    if (opts->write_out_file) {
        opts->out_file->writeln(user + ":UNKNOWN");
    }
}

void out_combo(string user, string pass, bool success) {
    if (!success) {
        out_combo(user);
        return;
    }

    fine(user + ":" + pass);
    if (opts->write_out_file) {
        opts->out_file->writeln(user + ":" + pass);
    }
}

void out_combo(LogonStatus *status) {
    if (status->success) {
        fine("Success!");
        fine("Password for " + string(status->user) + ": " + string(status->pass));
    } else {
        warn("Failure!");
    }
    fine("Status: " + string(status->msg) + " (" + itostr(status->rvalue) + ")");
}

void countdown() {
    if (opts->quiet || (opts->user_source == USER_GIVEN && opts->pass_source == PASS_GIVEN)) {
        return;
    }

    out("[*] Press enter anytime to show progress");

    thread_sleep(500);
    out(".");
    thread_sleep(500);
    out(".");
    thread_sleep(500);
    out(".");
    outln();
}

FILE* open_file(const char *filename) {
    return open_file(filename, "r");
}

FILE* open_file(const char *filename, const char* opts) {
    FILE* fileout;
    if (!(fileout = fopen(filename, opts))) {
        outln("Failed to open: " + string(filename));
        exit(1);
    }

    return fileout;
}

// http://stackoverflow.com/questions/478528/parsing-integer-to-string-c

int parse_int(const char *s, int *i) {
    char *ep;
    long l;

    l = strtol(s, &ep, 0);

    if (*ep != 0) {
        return 0;
    }

    *i = (int) l;
    return 1;
}

// http://www.intechgrity.com/c-program-replacing-a-substring-from-a-string/#

void strrep(char *str, const char *from, const char *to) {
    //a buffer variable to do all replace things
    char buffer[2048];
    //to store the pointer returned from strstr
    char *ch;

    // First exit condition
    if (!(ch = strstr(str, from))) {
        return;
    }

    //copy all the content to buffer before the first occurrence of the search string
    strncpy(buffer, str, ch - str);

    //prepare the buffer for appending by adding a null to the end of it
    buffer[ch - str] = 0;

    //append using sprintf function
    sprintf(buffer + (ch - str), "%s%s", to, ch + strlen(from));

    //empty o_string for copying
    str[0] = 0;
    strcpy(str, buffer);
    //pass recursively to replace other occurrences
    return strrep(str, from, to);
}

// http://stackoverflow.com/questions/19473093/int-to-char-c-help-me

const char* itostr(int a) {
    stringstream tmp;
    tmp << a;
    return tmp.str().c_str();
}
