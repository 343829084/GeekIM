#ifndef _FILE_H_
#define _FILE_H_

#include<fstream>
#include<string>

using namespace std;

class File{

public:
    static string getPath();
    static void setPath(string dirpath);
  //  static void setPath(const char *dirpth);
    bool open(string filepath);
    bool open(const char *filepath);

    void close();
    void type(const char* filepath,struct stat *sbuf);

    size_t size();
    bool read(void *buffer,size_t len);
    bool write(void *buffer,size_t len);
    char *get();

private:
    static string basepath;
    fstream file;

};

#endif
