#ifndef _HTTP_H_
#define _HTTP_H_

#include<map>
#include<string>

using namespace std;

#define HDR_MAXLEN 2048

#define REPLY_OK 200
#define REPLY_BADREQ 400
#define REPLY_NOENT 404
#define REPLY_INTERR 500
#define REPLY_NOTIMP 501

class Http{


public:
    static void init();
    static string getDescription(int reply);
private:
    static map<int,const char*> replyMap;

};

#endif
