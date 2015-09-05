#include "include/pluto.h"

void console_update() {
    if (!opts->do_mangling) {
        outfln("%i tries, %i/%i cracked @ %i tps", stat_tries, stat_cracks, stat_users, stat_tps);
    } else {
        outfln("%i tries (%i mpu), %i/%i cracked @ %i tps", stat_tries, stat_mpu, stat_cracks, stat_users, stat_tps);
    }
}
