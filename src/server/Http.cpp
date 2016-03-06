#include"common.h"

map<int,const char*> Http::replyMap;

void Http::init()
{
    replyMap[REPLY_OK] = "OK";
    replyMap[REPLY_BADREQ] = "Bad Request";
    replyMap[REPLY_NOENT] = "Not Found";
    replyMap[REPLY_INTERR] = "Internal Error";


}

string Http::getDescription(int reply)
{
    const char *description;
    description = replyMap[reply];
    return string(description);
}
