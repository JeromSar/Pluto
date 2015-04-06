#include "include/pluto.h"
#include <chrono> // high_resolution_clock::now()
#include <ctime> // time(), localtime()
#include <sstream> // stringstream

using namespace std::chrono;

long ms_timestamp() {
    return high_resolution_clock::now().time_since_epoch().count() / 1000000;
}

time_t ut_timestamp() {
    return time(0);
}

string format_time(time_t utime) {
    char buf[80];
    strftime(buf, sizeof (buf), "%a, %b %d %Y %H:%M:%S", localtime(&utime)); // http://en.cppreference.com/w/cpp/chrono/c/strftime
    return buf;
}

string get_formatted_time() {
    return format_time(ut_timestamp());
}
