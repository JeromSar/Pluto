#include  "include/pluto.h"
//
#include <iostream> // fopen()

void countdown() {
    if (opts->quiet) {
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
