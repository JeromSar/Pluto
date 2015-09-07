#ifndef PLUTO_H
#define	PLUTO_H

#include <conio.h> // _getch(), _kbhit()
#include <cstdio> // FILE
#include <string> // std::string
#include <functional> // std::function
#include <vector> // vector

// Passthrough
#include "base.h"
#include "util.h"
#include "winapi.h"

// console.cpp
string format_stats();

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
typedef std::function<void(char*, const char*) > rule;
typedef std::function<bool(char*) > callback;
vector<string>* mangle(const char *pass);
void mangle_init();

// optionparser.cpp
void parse_args(int argc, char **argv);

// stat.cpp
void update_tps(int tries);
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
int parse_int(const char *s, int *i);
void strrep(char *str, const char *from, const char *to);
const char* itostr(int x);
void verbose_combo(const char* user, const char* pass, bool filtered);
void out_combo(string user);
void out_combo(string user, string pass, bool success);
void out_combo(LogonStatus *status);

#endif

