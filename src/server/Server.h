#ifndef _SERVER_H_
#define _SERVER_H_
#include<vector>
#include"Socket.h"
#include"Thread.h"

using namespace std;

class Server{

public:
    Server(unsigned short port = 80);
    ~Server();
    bool start();
    void stop();
    bool accept();
    int getSockFd();

private:
     static void *handler(void *client);
     bool createThread(Socket *socket);

    Socket socket;
    vector<Thread *> threads;
    unsigned short port;
    

};
#endif
