#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

#include<event.h>
#include<event2/listener.h>
#include<event2/bufferevent.h>

#define SERVER_PORT 8888

void listener_cb(evconnlistener *listener,evutil_socket_t fd,
                struct sockaddr *sock,int socklen,void *arg);

void socket_read_cb(bufferevent *bev,void *arg);
void socket_event_cb(bufferevent *bev,short events,void *arg);

int main()
{
    struct sockaddr_in sin;
    memset(&sin,0,sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);

    event_base *base = event_base_new();
    evconnlistener *listener = evconnlistener_new_bind(base,listener_cb,base,LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,10,(struct sockaddr*)&sin,sizeof(struct sockaddr_in));
    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);

    return 0;
}


void listener_cb(evconnlistener *listener,evutil_socket_t fd,struct )
