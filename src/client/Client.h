#include<iostream>
#include<sstream>
#include<string>
#include<cstring>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>

#include<curses.h>

#include<pthread.h>

using namespace std;


struct threadArgs {
  int clientSock;
};

class Client{

public:
    void clearInputScreen();

    bool getUserInput(string& inputStr, bool isPwd);

    void displayMsg(string& msg);

    void prepareWindows();

    int openSocket (string hostName, unsigned short serverPort);

    bool SendMessage(int HostSock, string msg);

    string GetMessage(int HostSock, int messageLength);

    bool SendInteger(int HostSock, int hostInt);

    long GetInteger(int HostSocks);

    void* clientThread(void* args_p);

    bool hasAuthenticated (int hostSock, string &username);

    void DisplayData (int hostSock);

};
