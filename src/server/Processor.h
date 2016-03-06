#include<iostream>
#include<string>
#include"Common.h"
#include"Socket.h"

using namespace std;
using namespace wbinglib;


class Processor{

public:

    Processor(Socket *socket);

    ~Processor();

    void disconnect();

    void InstantMessage();

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

    int hasAuthenticated(int clientSock, string &userName);

    void broadcastMsg(string userName, string msg, bool isConnected);

    string GrabUsers(string userName);

    string GrabTime(string userName);

    string GrabJoke();

    string GrabPic();
private:
    Socket *socket;
};

