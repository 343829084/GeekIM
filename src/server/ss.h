#ifndef _Processor_H_
#define _Processor_H_

#include<string>


#include"Header.h"
#include"Http.h"
#include"Socket.h"

using namespace std;
using namespace wbinglib;

class Processor{

public:
    Processor(Socket *socket);
    ~Processor();
    void disconnect();
    size_t read(void *buffer , size_t len);
    size_t write(const void *buffer,size_t len);
    bool send(const void *buffer,size_t len,int reply = REPLY_OK);
    bool sendFile(string filepath);
    bool sendFile(const char *filepath);
    bool recvRequest(Header &header);
    bool handleError(int reply);
    bool sendDir(const char *filepath,File &file,Header &header);
    bool sendRegFile(const char *filepath,File &file,Header &header);
    bool sendHeader(Header &header);
private:

    Socket *socket;




};

#endif

