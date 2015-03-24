#include  "include/pluto.h"

void msg(string msg) {
    outln("# " + msg);
}

void action(string act) {
    outln("> " + act);
}

void interactive() {

    char *domain = MAKE_STR;
    char *user = MAKE_STR;
    char *pass = MAKE_STR;
    char *input = MAKE_STR;

    domain[0] = '.';
    domain[1] = 0;

    msg("Interactive " + NAME);
    msg("Type '?' for help");
    outln("");

    while (true) {
        out("# " + NAME + "$ ");
        gets(input);
        switch (input[0]) {

            case '?':
            {
                outln(
                        "# Interactive mode commands:\n"
                        "#    d <domain>    Set domain (Use  \".\" for local machine)\n"
                        "#    u <user>      Set username\n"
                        "#    p <pass>      Set password\n"
                        "#    x             Attempt logon\n"
                        "#    q             Quit\n");
                break;
            }

            case 'd':
            {
                strcpy(domain, input + 2);
                action("Domain set.");
                break;
            }

            case 'u':
            {
                strcpy(user, input + 2);
                action("User set.");
                break;
            }

            case 'p':
            {
                strcpy(pass, input + 2);
                action("Pass set.");
                break;
            }

            case 'x':
            {
                LogonStatus* status = verbose_logon(user, pass, domain);
                if (status->success) {
                    action("Login success!");
                } else {
                    action("Login Failure!");
                    action(status->msg);
                }
                outln("");
                break;
            }

            case 'q':
            {
                free(domain);
                free(user);
                free(pass);
                free(input);
                return;
            }

            default:
            {
                msg("Unknown command, type '?' to view commands.\n");
            }
        }
    }
}
