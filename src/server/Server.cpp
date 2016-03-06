#include"common.h"
#include<event.h>

using namespace std;
static unsigned short g_port;

Server::Server(unsigned short port)
{
    this->port = port;
    g_port = port;

}

Server::~Server()
{
    stop();

}

void *Server::handler(void *cProcessor)
{
    Processor *processor = (Processor *)cProcessor;
    processor->InstantMessage();
    cout << "Disconnecting..." << endl;
    processor->disconnect();
    delete processor;
    return NULL;
}

bool Server::start()
{

    return socket.init_listenfd(port);

}

void Server::stop()
{
    vector<Thread *>::iterator it;
    socket.destroy();
    for(it=threads.begin();it!=threads.end();++it){
        Thread *thread = *it;
        cout << "Destroying thread ..." << endl;
        thread->destroy();
        thread->join();
        delete thread;
    }

    threads.clear();
}

bool Server::createThread(Socket *socket)
{
    Processor *processor;
    Thread *thread;

    bool res;

    processor = new Processor(socket);
    if(!processor)
        return false;

    thread = new Thread();
    if(!thread)
        return false;
    res = thread->create(handler,processor);
    if(!res)
        return false;

    threads.push_back(thread);
    return true;
}
int Server::getSockFd()
{
    return socket.getSockFd();
}

bool Server::accept()
{

    Socket *processor;
    processor = socket.accept();
    cout << "Waiting for incoming connection..." << endl;

    if(!processor){
        cerr << "socket.accept() error!"<< endl;
        return false;
    }

    return createThread(processor);

}
