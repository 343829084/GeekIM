#include"common.h"

using namespace wbinglib;


Socket::Socket()
{
    sockFd = -1;

}
Socket::Socket(int fd)
{
    sockFd = fd;

}


Socket::~Socket()
{
    destroy();
}

bool Socket::create()
{
    destroy();
    sockFd = socket(AF_INET,SOCK_STREAM,0);
    if(sockFd < 0)
        return false;
    
    return true;
}

void Socket::destroy()
{
    if(sockFd>=0)
        shutdown(sockFd,SHUT_RDWR);
    sockFd = -1;
}

bool Socket::bind(int port)
{
    struct sockaddr_in sockAddr;
    int res;

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    res = ::bind(sockFd,(SA *)&sockAddr,sizeof(sockAddr));

    if((res==-1))
        return false;
    return true;
}

bool Socket::listen(int listenq)
{
    int res;
    res = ::listen(sockFd,listenq);
    if(res == -1)
        return false;
    return true;

}
void Socket::setnonblocking()
{
    int opts;
    opts=fcntl(sockFd,F_GETFL);
    if(opts<0)
    {
      perror("fcntl(sockFd,GETFL)");
      exit(1);
    
    }
   opts = opts|O_NONBLOCK;
   if(fcntl(sockFd,F_SETFL,opts)<0)
   {
    perror("fcntl(sockFd,SETFL,opts)");
    exit(1);
   
   }
   return;

}


bool Socket::init_listenfd(short port)
{
    bool flag;
    flag = create();
    if(!flag)
    {
        cerr << "socket create  error!" << endl;
        return false;
    }
    flag = bind(port);
    if(!flag)
    {
        cerr << "socket bind error!"<<endl;
        return false;

    }
    flag = listen(LISTENQ);
    if(!flag)
    {
        cerr << "socket listen error!" << endl;
        return false;
    }
    
    return true;

}
Socket *Socket::accept()
{
    struct sockaddr_in sockAddr;
    socklen_t sockLen;
    int fd;

    sockLen = sizeof(sockAddr);
    fd = ::accept(sockFd,(struct sockaddr *)&sockAddr,&sockLen);
    if(fd == -1)
        return NULL;
    return (new Socket(fd));
}

size_t Socket::send(const void *buffer,size_t len)
{
    return ::send(sockFd,buffer,len,MSG_NOSIGNAL);

}


size_t Socket::receive(void *buffer,size_t len)
{
    return ::recv(sockFd,buffer,len,0);
}




