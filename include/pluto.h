#ifndef PLUTO_H
#define	PLUTO_H

#include <conio.h> // _getch(), _kbhit()
#include <cstdio> // FILE
#include <string> // std::string
#include <functional> // std::function

// Passthrough
#include "base.h"
#include "util.h"
#include "winapi.h"

using namespace std;

// console.cpp
void console_update();
void console_clear();
void console_clearline();

// cracker.cpp
void crack();

// filter.cpp
bool filter_pass(const char *pass);

// interactive.cpp
void interactive();

// main.cpp
extern Options* opts;
void show_help(char **argv);

// mangler.cpp
typedef function<void(char*, const char*) > rule;
typedef function<bool(char*) > callback;
void mangle_init();
bool mangle(char *user, char *pass, char *domain);

// optionparser.cpp
void parse_args(int argc, char **argv);

// stat.cpp
void update_tps(int tries, int millis);
void update_mpu(int mangles);
extern time_t stat_start;
extern int stat_users;
extern int stat_tries;
extern int stat_cracks;
extern int stat_tps; // tries per sec
extern int stat_mpu; // mangles per user

// time.cpp
long ms_timestamp();
time_t ut_timestamp();
string format_time(time_t utime);
string get_formatted_time();

// util.cpp
void countdown();
FILE* open_file(const char *filename);
FILE* open_file(const char *filename, const char* opts);
void out_combo(string user);
void out_combo(string user, string pass, bool success);
void out_combo(LogonStatus *status);
int parse_int(const char *s, int *i);
void strrep(char *str, const char *from, const char *to);
const char* itostr(int x);

#endif

