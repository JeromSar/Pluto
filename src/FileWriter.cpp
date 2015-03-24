#include "include/FileWriter.h"
#include "include/pluto.h"

FileWriter::FileWriter(string name) {
    file = open_file(name.c_str(), "a");
}

FileWriter::~FileWriter() {
    close();
}

void FileWriter::write(string data) {
    fprintf(file, "%s", data.c_str());
}

void FileWriter::writeln(string data) {
    write(data + "\n");
}

void FileWriter::close() {
    fclose(file);
}

