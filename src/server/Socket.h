#ifndef _SOCKET_H_
#define _SOCKET_H_

namespace wbinglib{


class Socket{

public:
    Socket();
    Socket(int fd);
    ~Socket();
    void setnonblocking();
    bool create();
    void destroy();
    bool bind(int port);
    bool listen(int listenq);
    bool init_listenfd(short port);
    Socket *accept();
    size_t send(const void *buffer,size_t len);
    size_t receive(void *buffer,size_t len);
    int getSockFd()
    {
        return this->sockFd;
    }
private:
    int sockFd;

};

}

#endif
