#include<string>
#include<ctime>




struct threadArgs {
  int clientSock;
};

struct User {
    std::string username;
    std::string password;
    time_t timeConnected;
    bool isConnected;
};

struct Msg {
    std::string to;
    std::string from;
    std::string msg;
    std::string cmd;
};



