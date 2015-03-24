#ifndef FILEWRITER_H
#define	FILEWRITER_H

#include <string>
#include <cstdio>

using namespace std;

class FileWriter {
public:
    FileWriter(string name);

    ~FileWriter();

    void write(string data);

    void writeln(string data);

    void close();

private:
    FILE *file;
};

#endif

