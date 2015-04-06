#include "include/pluto.h"
#include "include/rlutil.h"

Options* opts = new Options;

void show_help(char **argv) {
    outln(
            "Usage: " + string(argv[0]) + " [options] <user source> <password source>\n"
            "\n"
            "Options:\n"
            "   -?, -h, --help               Shows this usage screen\n"
            "   -v, --verbose                Outputs verbosely if possible\n"
            "   -q, --quiet                  Minimal mode. Minimizes output and skips delay\n"
            "   -i, --interactive            Interactive mode\n"
            "   -d, --domain <domain>        Specifies the external domain to use\n"
            "   -o, --output <file>          Specifies output file. Output will be appended to an existing file.\n"
            "   -s, --single                 Single mode. Stops after one successful crack\n"
            "   -m, --mangle <options>       Mangle passwords. Tries different passwords variations per password\n"
            "   -f, --filter <options>       Filter passwords. Skips passwords that don't match the specified options\n"
            "   -t, --max-tries <max>        Specifies the maximum amount of tries per user\n"
            "   --enter-info                 Shows cracking progress when pressing enter. Press Ctrl+C to stop cracking\n"
            "\n"
            "Mangling:\n"
            "   -m, --mangle [a]             Mangles passwords with the specified rules\n"
            "     p                          Uses standard prefixes\n"
            "     l                          Uses l33tsp3@k rules\n"
            "     c                          Uses capitalization rules\n"
            "     s                          Uses standard suffixes\n"
            "     d<digit>                   Sets mangling rule depth. Default is 3. Use 0 for max.\n"
            "     a                          Uses all combinations of rules, prefixes and suffixes, may be slow\n"
            "\n"
            "Filtering:\n"
            "   -f [s<digit>][d<digit>][D<digit>][l<digit>][a<digit>][A<digit>][p<digit>]\n"
            "     s<digit>                   Minimum amount of characters\n"
            "     d<digit>                   Minimum amount of digits\n"
            "     D<digit>                   Minimum amount of digits or punctuation\n"
            "     a<digit>                   Minimum amount of alpha letters\n"
            "     l<digit>                   Minimum amount of alpha lower case letters\n"
            "     u<digit>                   Minimum amount of alpha upper case letters\n"
            "     p<digit>                   Minimum amount of punctuation characters or spaces\n"
            "\n"
            "User sources:\n"
            "   -u, --user <user>            Specifies a direct user\n"
            "   -l, --user-list <file>       Specifies a list of users to use\n"
            "\n"
            "Password sources:\n"
            "   -p, --pass <pass>            Specifies a direct password\n"
            "   -w, --pass-list <file>       Specifies a list of passwords to use\n"
            "   -b, --brute <options>        Specifies a bruteforce attack should be used\n"
            "\n"
            "Bruteforcing:\n"
            "   -b, --brute [l][u][p][d][s]   Uses a bruteforce attack to crack the password\n"
            "     l                           Includes lowercase alphabetical characters\n"
            "     u                           Includes uppercase alphabetical characters\n"
            "     d                           Includes digit characters\n"
            "     p                           Includes punctuation characters\n"
            "     s                           Includes space\n"
            "   -bmn, --brute-min <min>       Specifies the minimum password size for the bruteforce attack. Default is 3.\n"
            "   -bmx, --brute-max <max>       Specifies the maximum password size for the bruteforce attack. Default is 6.\n"
            "   --brute-start <pass>          Specifies the brute password with which to start cracking\n"
            );
}

int main(int argc, char **argv) {

    /* //Debug
    int i = 0;

    while (true) {
        thread_sleep(1000);

        outln("YEAH!");

        // Clear console
        short x = rlutil::getX();
        short y = rlutil::getY();
        console_clearline();
        outln();
        console_clearline();
        outln();
        console_clearline();
        rlutil::locate(x, y);

        // Print one line out
        x = rlutil::getX();
        y = rlutil::getY();
        outln();
        outfln("Test %i (%i, %i)", i++, x, y);
        rlutil::locate(x, y);
    }
    return 0; */

    stat_start = ut_timestamp();

    parse_args(argc, argv);

    if (!opts->quiet) {
        outln();
        info(NAME + " version " + VERSION + " by " + AUTHOR);
        info("Copyright (C) 2014-2015 " + AUTHOR + ". All rights reserved.");
        outln();
        info("Started on " + get_formatted_time());
        outln();
    }

    if (opts->help) {
        show_help(argv);
        exit(0);
    }

    if (opts->interactive) {
        interactive();
        exit(0);
    }

    crack();

    outln();
    info("Finished at " + get_formatted_time());
    //info("Elapsed time: ");
    return 0;
}
