#include"common.h"


class Epoll
{

public:
    
    bool  create(int size);
    bool  ctladd(int fd);
    bool  ctlmod(int fd);
    bool  ctldel(int fd);
    int   wait();

private:

    int epfd;
    epoll_event ev,*events;
    int maxevents;
    int timeout;






}
