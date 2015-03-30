#include "include/pluto.h"
#include "include/FileIterator.h"
#include "include/SingleIterator.h"
#include "include/FileWriter.h"
#include "include/BruteIterator.h"

void parse_args(int argc, char **argv) {
    if (argc == 1) {
        show_help(argv);
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        string opt = argv[i];

        if (opt.length() < 2) {
            outfln("Invalid argument: %s", argv[i]);
            exit(1);
        }

        if (opt == "-?" || opt == "-?" || opt == "--help") {
            opts->help = true;
            return;
        }

        if (opt == "-i" || opt == "--interactive") {
            opts->interactive = true;
            return;
        }

        if (opt == "-s" || opt == "--single") {
            opts->single = true;
            continue;
        }

        if (opt == "-v" || opt == "--verbose") {
            opts->verbose = true;
            continue;
        }

        if (opt == "-q" || opt == "--quiet") {
            opts->quiet = true;
            continue;
        }

        if (opt == "--enter-info") {
            opts->enter_info = true;
            continue;
        }

        // Beyond this point, args require opts
        if (i + 1 >= argc) {
            outfln("Parameter required for: %s", argv[i]);
            exit(1);
        }

        if (opt == "-u" || opt == "--user") {
            opts->users = new SingleIterator(argv[++i]);
            opts->user_source = USER_GIVEN;
            continue;
        }

        if (opt == "-l" || opt == "--user-list") {
            FILE *user_file = open_file(argv[++i]);
            opts->users = new FileIterator(user_file);
            opts->user_source = USER_FILE;
            continue;
        }

        if (opt == "-p" || opt == "--pass") {
            char* pass = argv[++i];
            opts->passwords = new SingleIterator(pass);
            opts->pass_source = PASS_GIVEN;
            continue;
        }

        if (opt == "-w" || opt == "--pass-list") {
            FILE *pass_file = open_file(argv[++i]);
            opts->passwords = new FileIterator(pass_file);
            opts->pass_source = PASS_FILE;
            continue;
        }

        if (opt == "-d" || opt == "--domain") {
            opts->domain = argv[++i];
            continue;
        }

        if (opt == "-o" || opt == "--output") {
            opts->write_out_file = true;
            opts->out_file = new FileWriter(argv[++i]);
            continue;
        }

        if (opt == "-t" || opt == "--max-tries") {
            opts->limit_tries = true;
            if (!parse_int(argv[++i], &opts->max_tries)) {
                outfln("Invalid number: %s", argv[i]);
                exit(1);
            }
            continue;
        }

        if (opt == "-m" || opt == "--mangle") {
            opts->mangle_pass = true;

            const int optlen = strlen(argv[++i]);

            for (int j = 0; j < optlen; j++) {
                char c = argv[i][j];

                switch (c) {
                    case 'a':
                        opts->mangle_opts->all = true;
                        break;
                    default:
                        outfln("Unrecognized option: %c", c);
                        exit(1);
                        return;
                }
            }

            continue;
        }

        if (opt == "-f" || opt == "--filter") {
            opts->filter_pass = true;

            const int optlen = strlen(argv[++i]);

            for (int j = 0; j < optlen; j += 2) {
                char c = argv[i][j];
                int l;

                if (!parse_int(new char[2] {
                        argv[i][j + 1], 0 }, &l)) {
                outfln("Could not parse int: %s", argv[i][j + 1]);
                exit(1);
            }

                FilterOptions *fopts = opts->filter_opts;

                switch (c) {
                    case 's':
                        fopts->min_size = l;
                        break;
                    case 'd':
                        fopts->min_digit = l;
                        break;
                    case 'D':
                        fopts->min_digit_or_punct = l;
                        break;
                    case 'l':
                        fopts->min_alpha = l;
                        break;
                    case 'a':
                        fopts->min_lower_alpha = l;
                        break;
                    case 'A':
                        fopts->min_upper_alpha = l;
                        break;
                    case 'p':
                        fopts->min_punct = l;
                        break;
                    default:
                        outfln("Unrecognized option: %c", c);
                        exit(1);
                        break;
                }
            }
            continue;
        }

        if (opt == "-b" || opt == "--brute") {
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
                        outfln("Unrecognized option: %c", c);
                        exit(1);
                        return;
                }

                opts->passwords = new BruteIterator(opts->brute_opts);
            }
            continue;
        }

        if (opt == "-bmn" || opt == "--brute-min") {
            if (!parse_int(argv[++i], &opts->brute_opts->min_len)) {
                outfln("Invalid number: %s", argv[i]);
                exit(1);
            }
            // Ensure password length is in range
            opts->brute_opts->max_len = (opts->brute_opts->min_len > opts->brute_opts->max_len ? opts->brute_opts->min_len : opts->brute_opts->max_len);
            continue;
        }

        if (opt == "-bmx" || opt == "--brute-max") {
            if (!parse_int(argv[++i], &opts->brute_opts->max_len)) {
                outfln("Invalid number: %s", argv[i]);
                exit(1);
            }
            // Ensure password length is in range
            opts->brute_opts->min_len = (opts->brute_opts->min_len > opts->brute_opts->max_len ? opts->brute_opts->max_len : opts->brute_opts->min_len);
            continue;
        }

        if (opt == "--brute-start") {
            opts->brute_opts->has_start_pass = true;
            strcpy(opts->brute_opts->start_pass, argv[++i]);
            continue;
        }

        outfln("Invalid argument: %s", argv[i]);
        exit(1);
    }

    return;
}