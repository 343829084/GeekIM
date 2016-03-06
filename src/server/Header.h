#ifndef _HEADER_H_
#define _HEADER_H_

#include"common.h"
using namespace std;

class Header{

public:
    Header();
    Header(string header);
    void clear();

void init();
    string generate();
    bool getType(string &type);
    bool getPath(string &path);
    void set(string header);
    void set(const char *header);
    void addReply(int reply);
    void addLength(size_t len);
    void addDate();
    void addServer();
    void addEnd();
    void addType(const char *path); 
    string get_mime_type(const char *path);
    static map<string,string> mimeTypes;
private:
    stringstream header;


};


#endif
