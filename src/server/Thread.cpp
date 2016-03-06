#include"Thread.h"
using namespace wbinglib;
Mutex::Mutex()
{
    pthread_mutex_init(&mutex,NULL);

}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&mutex);
}

bool Mutex::lock()
{
    int res;
    res = pthread_mutex_lock(&mutex);
    if(res)
        return false;
    return true;

}

bool Mutex::unlock()
{
    int res;
    res = pthread_mutex_unlock(&mutex);
    if(res)
        return false;
    return true;
}

Thread::~Thread()
{
    destroy();
}

bool Thread::create(void *(*routine)(void *),void *arg)
{
    int res;

    res = pthread_create(&thread,NULL,routine,arg);
    if(res)
        return false;
    return true;
}

bool Thread::destroy()
{
    int res;
    res = pthread_cancel(thread);
    if(res)
        return false;
    return true;
}

bool Thread::join()
{
    int res;
    res = pthread_join(thread,NULL);
    if(res)
        return false;
    return true;
}

void Thread::Exit(void *ret)
{
    pthread_exit(ret);
}

