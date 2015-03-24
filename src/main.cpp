#include "include/pluto.h"
#include "include/SingleIterator.h"
#include "include/FileIterator.h"
#include <memory>

Options* opts = new Options;

void show_help(char **argv) {
    outln(
            "Usage: " + string(argv[0]) + " [options] <user source> <password source>\n"
            "\n"
            "Options:\n"
            "   -?                   Shows this usage method\n"
            "   -v                   Designates that wlpc should output verbosely\n"
            "   -m                   Minimal mode. Minimises output and skips delay\n"
            "   -i                   Interactive mode\n"
            "   -d <domain>          Specifies the external domain to use\n"
            "   -o <file>            Specifies output file. Output will be appended to an existing file."
            "   -s                   Stops after one successful crack\n"
            "\n"
            "User sources:\n"
            "   -u <user>            Specifies a direct user\n"
            "   -l <userlist>        Specifies a list of users to use\n"
            "\n"
            "Password sources:\n"
            "   -p <pass>            Specifies a direct password\n"
            "   -w <wordlist>        Specifies a list of passwords to use\n"
            "   -b <chars> [options] Specifies a bruteforce attack should be used\n"
            "\n"
            "Bruteforcing:\n"
            "   -b [l][u][p][d][s]   Uses a bruteforce attack to crack the password\n"
            "      l                   Includes lowercase alphabetical characters\n"
            "      u                   Includes uppercase alphabetical characters\n"
            "      d                   Includes digit characters\n"
            "      p                   Includes punctuation characters\n"
            "      s                   Includes space\n"
            "   -bmn <minimum>       Specifies the minimum password size for the bruteforce attack\n"
            "   -bmx <maximum>       Specifies the maximum password size for the bruteforce attack\n"
            );
}

int main(int argc, char **argv) {

    outln("");

    parse_args(argc, argv);

    if (!opts->minimal) {
        outln("# " + NAME + " version " + VERSION + " by " + AUTHOR);
        outln("# Copyright (C) 2014-2015 " + AUTHOR + ". All rights reserved.");
        outln("");
    }

    if (opts->help) {
        show_help(argv);
        exit(0);
    }

    if (opts->interactive) {
        interactive();
        exit(0);
    }

    // Bruteforce
    if (opts->pass_source == PASS_BRUTEFORCE) {
        brute_attack();
        return 0;
    }

    // Single mode
    single_attack();
    return 0;
}
