#include "include/pluto.h"
#include "include/FileIterator.h"
#include "include/SingleIterator.h"
#include "include/FileWriter.h"

// http://stackoverflow.com/questions/478528/parsing-integer-to-string-c
int parseInt(const char *s, int *i) {
    char *ep;
    long l;

    l = strtol(s, &ep, 0);

    if (*ep != 0) {
        return 0;
    }

    *i = (int) l;
    return 1;
}

void parse_args(int argc, char **argv) {
    if (argc == 1) {
        show_help(argv);
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        string opt = argv[i];

        if (strlen(argv[i]) < 2) {
            outfln("Invalid argument: %s", argv[i]);
            exit(1);
        }

        if (opt == "-?") {
            opts->help = true;
            return;
        }

        if (opt == "-i") {
            opts->interactive = true;
            return;
        }

        if (opt == "-s") {
            opts->stop = true;
            continue;
        }

        if (opt == "-v") {
            opts->verbose = true;
            continue;
        }

        if (opt == "-m") {
            opts->minimal = true;
            continue;
        }

        // Beyond this point, args require opts
        if (i + 1 >= argc) {
            outfln("Parameter required for: %s", argv[i]);
            exit(1);
        }

        if (opt == "-u") {
            opts->users = new SingleIterator(argv[++i]);
            opts->user_source = USER_GIVEN;
            continue;
        }

        if (opt == "-l") {
            FILE *user_file = open_file(argv[++i]);
            opts->users = new FileIterator(user_file);
            opts->user_source = USER_FILE;
            continue;
        }

        if (opt == "-p") {
            char* pass = argv[++i];
            opts->passwords = new SingleIterator(pass);
            opts->pass_source = PASS_GIVEN;
            continue;
        }

        if (opt == "-w") {
            FILE *pass_file = open_file(argv[++i]);
            opts->passwords = new FileIterator(pass_file);
            opts->pass_source = PASS_FILE;
            continue;
        }

        if (opt == "-d") {
            opts->domain = argv[++i];
            continue;
        }

        if (opt == "-o") {
            opts->write_out_file = true;
            opts->out_file = new FileWriter(argv[++i]);
            continue;
        }

        if (opt == "-b") {
            opts->pass_source = PASS_BRUTEFORCE;

            const int optlen = strlen(argv[++i]);

            for (int j = 0; j < optlen; j++) {
                char c = argv[i][j];

                switch (c) {
                    case 'l':
                        opts->brute_opts->lower = true;
                        break;
                    case 'u':
                        opts->brute_opts->upper = true;
                        break;
                    case 'd':
                        opts->brute_opts->digit = true;
                        break;
                    case 's':
                        opts->brute_opts->space = true;
                        break;
                    case 'p':
                        opts->brute_opts->punct = true;
                        break;
                    default:
                        outfln("Not a valid character type: %c", c);
                        return;
                }
            }

            continue;
        }

        if (opt == "-bmn") {
            if (!parseInt(argv[++i], &opts->brute_opts->min_len)) {
                outfln("Invalid number: %s", argv[i]);
                exit(1);
            }
            continue;
        }

        if (opt == "-bmx") {
            if (!parseInt(argv[++i], &opts->brute_opts->max_len)) {
                outfln("Invalid number: %s", argv[i]);
                exit(1);
            }
            continue;
        }

        outfln("Invalid argument: %s", argv[i]);
        exit(1);
    }

    return;
}