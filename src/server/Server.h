#include<iostream>
#include<sstream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<tr1/unordered_map>
#include<deque>
#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include<pthread.h>

using namespace std;

class Server{

public:
    Server();
    ~Server();
    void* clientThread(void* args_p);

    void InstantMessage(int clientSock);

    bool SendMessage(int HostSock, string msg);

    string GetMessage(int HostSock, int messageLength);

    bool SendInteger(int HostSock, int hostInt);

    long GetInteger(int HostSocks);

    void addToMsgQueue(Msg newMsg);

    void processMsg(string &msg, string &cmdName, string &userTo);

    void SaveMsg(string msg, string userFrom);

    string GetMsgs(string username);

    void setUserDisconnected (string username);

    void setUserConnected (User newUser);

    bool isUserConnected (User newUser);

    bool doesUserExist (string username);

    void addToUsersList (User newUser);

    bool loginUser (string username, string password);

    bool hasAuthenticated(int clientSock, string &userName);

    void broadcastMsg(string userName, string msg, bool isConnected);

    string GrabUsers(string userName);

    string GrabTime(string userName);

    string GrabJoke();

    string GrabPic();
};

