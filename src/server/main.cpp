#include"common.h"
#include<event.h>

using namespace std;

static Server *server = NULL;
struct event_base* base;

void Quit(int dummy)
{
    cout << "Terminating ..." << endl;
    server->stop();
    delete server;
    exit(0);
}

void socket_accept(int fd, short events, void *arg)
{
    server->accept();
}

int setnonblock(int fd)
{
    int flags; 
    flags = fcntl(fd, F_GETFL);
    if (flags < 0)
        return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0)
        return -1;
    return 0;

}

int main(int argc,char **argv)
{
    short port;
    int res;

    Http::init();
    if(argc!=2)
    {

    cerr << "usage: <cmd>   <port>"<< endl;
    return 1;

    }

    port = atoi(argv[1]);
    server = new Server(port);
    if(!server){
        perror("Server could Not be create!");
        return 1;
    }

    res = server->start();
    if(!res){
        perror("Server could Not be Started!");
        return 1;
    }

    int yes=1;
    signal(SIGINT,Quit);
    int sockfd = server->getSockFd();
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (setnonblock(sockfd) < 0)
        perror("failed to set server socket to non-blocking");

    base = event_base_new(); 
    struct event *ev_sockfd = event_new(base, sockfd,  EV_READ | EV_PERSIST,  socket_accept, NULL);  
    event_add(ev_sockfd, NULL);
    event_base_dispatch(base);  

    //while(server->accept());
    return 0;
}
