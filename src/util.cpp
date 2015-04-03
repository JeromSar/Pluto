#include  "include/pluto.h"

#include <iostream> // fopen()
#include <sstream> // ostringstream

void countdown() {
    if (opts->quiet || (opts->user_source == USER_GIVEN && opts->pass_source == PASS_GIVEN)) {
        return;
    }

    if (opts->enter_info) {
        out("Press enter anytime to show progress");
    } else {
        out("Press enter anytime to stop");
    }

    Sleep(500);
    out(".");
    Sleep(500);
    out(".");
    Sleep(500);
    out(".\n\n");
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

void out_combo(string user) {
    outln(user + ":UNKNOWN");
    if (opts->write_out_file) {
        opts->out_file->writeln(user + ":UNKNOWN");
    }
}

void out_combo(string user, string pass, bool success) {
    if (!success) {
        out_combo(user);
        return;
    }

    outln(user + ":" + pass);
    if (opts->write_out_file) {
        opts->out_file->writeln(user + ":" + pass);
    }
}

void out_combo(LogonStatus *status) {

    if (status->success) {
        outln("Success!");
        outfln("Password for %s: %s", status->user, status->pass);
        outfln("Status: %s (%d)", status->msg, static_cast<int> (status->rvalue));
    } else {
        outln("Failure!");
        outfln("Status: %s (%d)", status->msg, static_cast<int> (status->rvalue));
    }
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
