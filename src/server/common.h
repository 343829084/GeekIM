#ifndef _COMMON_H_
#define _COMMON_H_

#include<cstdio>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<ctime>
#include<cstdlib>
#include<signal.h>
#include<string.h>
#include<errno.h>
#include<dirent.h>
#include<map>
#include<sys/stat.h>


#include"Thread.h"
#include"Http.h"
#include"File.h"
#include"Header.h"
#include"Processor.h"
#include"Server.h"
#include"Socket.h"
#include"Thread.h"
#include"main.h"

#define BUF_MAXLEN 1024
#define LISTENQ 1024
typedef struct sockaddr SA;




#endif
