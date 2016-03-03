#include<iostream>
#include<string>
#include"Common.h"

using namespace std;

class Server{

public:

    Server(int t_port);

    ~Server();

    void* clientThread(void* args_p);

    static void * static_clientThread(void * void_this)
    {
        Server * thread_this = static_cast<Server*>(void_this);
        return thread_this->clientThread(thread_this->params);
    }

     void Start();

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
private:
    int m_port;
    void* params;
};

