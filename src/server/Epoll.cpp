#include"common.h"


bool Epoll::create(int size)
{
    epfd = epoll_create(size);
}

bool Epoll::ctlAdd(int fd)
{

}
