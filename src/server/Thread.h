#ifndef _THREAD_H_
#define _THREAD_H_

#include<pthread.h>

namespace wbinglib{


class Mutex{

public:
    Mutex();
    ~Mutex();
    bool lock();
    bool unlock();
private:
    pthread_mutex_t mutex;


};

class Thread{

public:
    ~Thread();
    bool create(void *(*routine)(void *),void *arg);
    bool destroy();
    bool join();
    static void Exit(void *ret);
private:
    pthread_t thread;

};

}
#endif
