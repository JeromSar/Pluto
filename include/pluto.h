#ifndef PLUTO_H
#define	PLUTO_H

#include "util.h" // OptionSet
#include "winapi.h" // LogonStatus
#include <conio.h> // _getch(), _kbhit()
#include <cstdio> // FILE
#include <string> // std::string
#include <set> // std::set
#include <functional> // std::function

// Allow strings to be used more freely
using namespace std;

// console.h
inline void out(string str);
inline void outln(string str);
inline void outfln(string str);
inline void outln();

// util.h
inline bool enter_down();

// winapi.h
inline LogonStatus* verbose_logon(char* user, char* pass, char* domain);
inline bool fast_logon(char* user, char* pass, char* domain);

// cracker.cpp
void crack();

// filter.cpp
bool filter_pass(char *pass);

// interactive.cpp
void interactive();

// main.cpp
extern Options* opts;
void show_help(char **argv);

// mangler.cpp
typedef std::function<void(char*, const char*) > rule;
typedef std::function<bool(char*) > callback;
void mangle_init();
bool mangle(char *user, char *pass, char *domain);

// optionparser.cpp
void parse_args(int argc, char **argv);

// stat.cpp in stat.h

// util.cpp
void countdown();
FILE* open_file(const char *filename);
FILE* open_file(const char *filename, const char* opts);
void out_combo(string user);
void out_combo(string user, string pass, bool success);
void out_combo(LogonStatus *status);
int parse_int(const char *s, int *i);
void strrep(char *str, const char *from, const char *to);

#endif

