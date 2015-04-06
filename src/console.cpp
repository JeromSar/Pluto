#include "include/pluto.h"
#include "include/rlutil.h"

int x, y;

void console_clearline() {
    int x = rlutil::getX();
    int y = rlutil::getY();


    rlutil::locate(0, y);
    int cols = rlutil::tcols();

    string str;
    for (int i = 1; i < cols; i++) {
        str += " ";
    }
    out(str);
    rlutil::locate(x, y);
}

void console_clear() {
    int x = rlutil::getX();
    int y = rlutil::getY();

    //outfln("CLEARING %i, %i", x, y);

    // Clear next lines
    console_clearline();
    outln();
    console_clearline();

    rlutil::locate(x, y);
}

void console_update() {
    x = rlutil::getX();
    y = rlutil::getY();

    console_clear();

    rlutil::locate(x, y + 1);

    if (!opts->do_mangling) {
        outfln("%i tries, %i/%i cracked @ %i tps", stat_tries, stat_cracks, stat_users, stat_tps);
    } else {
        outfln("%i tries (%i mpu), %i/%i cracked @ %i tps", stat_tries, stat_mpu, stat_cracks, stat_users, stat_tps);
    }

    // Restore position
    rlutil::locate(x, y);
}
