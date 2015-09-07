#include <sstream> // stringstream

#include "include/pluto.h"

string format_stats() {
    std::stringstream fmt;

    fmt << stat_tries << " tries";
    if (opts->do_mangling) {
        fmt << " (" << stat_mpu << " mpu), ";
    } else {
        fmt << ", ";
    }
    fmt << stat_cracks << "/" << stat_users << " cracked @ " << stat_tps + " tps";
    return fmt.str();
}

