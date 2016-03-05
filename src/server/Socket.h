#ifndef _SOCKET_H_
#define _SOCKET_H_


class Socket{

public:
    Socket();
    Socket(int fd);
    ~Socket();
    bool create();
    void destroy();
    bool bind(int port);
    bool listen(int listenq);
    bool init_listenfd(short port);
    Socket *accept();
    size_t send(const void *buffer,size_t len);
    size_t receive(void *buffer,size_t len);
    int getsockFd();
private:
    int sockFd;

};



#endif
