#include "include/pluto.h"

time_t stat_start; // start time
int stat_users = 0;
int stat_tries = 0;
int stat_cracks = 0;
int stat_tps = 0; // tries per sec
int stat_mpu = 0; // mangles per user

void update_mpu(int mangles) {
    static int m_mangles = 0;
    static int m_users = 0;
    m_mangles += mangles;
    stat_mpu = m_mangles / ++m_users;
}

void update_tps(int tries, int millis) {
    static int tps_tries = 0;
    static int tps_millis = 1;
    tps_tries += tries;
    tps_millis += millis;
    double tps_ms = (double) tps_tries / tps_millis;
    stat_tps = tps_ms * 1000;
}

