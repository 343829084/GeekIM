#include<iostream>
#include<cstdio>
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
#include"Server.h"

using namespace std;

const int MAXPENDING = 20;
tr1::unordered_map<string, User> UsersList;
deque<Msg> MsgQueue;
pthread_mutex_t MsgQueueLock;
pthread_mutex_t UserListLock;
int MsgStatus = pthread_mutex_init(&MsgQueueLock, NULL);
int UserStatus = pthread_mutex_init(&UserListLock, NULL);

Server::Server(unsigned short t_port):m_port(t_port)
{

}
Server::~Server(void)
{
    
}

void Server::Start()
{
#ifdef _wbing
    printf("[wbing]                  1\n");
    int conn_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    printf("conn_socket = %d\n",conn_socket);
    if(conn_socket < 0)
    {
        cerr << "Error with socket." << endl;
        exit(-1);
    }
    printf("[wbing]                  2\n");
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(m_port);
    printf("port=%d\n",m_port);
    printf("[wbing]                  3\n");

    int listen_status = listen(conn_socket,MAXPENDING);
    if(listen_status < 0)
    {
        cerr << "Error with listening." << endl;
        exit(-1);

    }
    cout << endl << endl << "SERVER: Ready to accept connections." << endl;
    printf("[wbing]                  4\n");
#endif
    socket.init_listenfd(m_port);

    while(true){
    printf("[wbing]                  4.1\n");
        struct sockaddr_in clientAddress;
        socklen_t addrLen = sizeof(clientAddress);
    printf("[wbing]                  4.2\n");
        int clientSocket = accept(socket.getsockFd(), (struct sockaddr*) &clientAddress, &addrLen);
    printf("[wbing]                  4.3\n");
        if (clientSocket < 0) {
          cerr << "Error accepting connections." << endl;
          exit(-1);
        }
    printf("[wbing]                  5\n");

        struct threadArgs* args_p = new threadArgs;
        args_p -> clientSock = clientSocket;
        pthread_t tid;
        //int threadStatus = pthread_create(&tid, NULL,clientThread,(void*)(args_p));

    printf("[wbing]                  6\n");
        int threadStatus = pthread_create(&tid, NULL,&static_clientThread,this);
    printf("[wbing]                  7\n");
        if (threadStatus != 0){
          cerr << "Failed to create child process." << endl;
          close(clientSocket);
          pthread_exit(NULL);
        }
    printf("[wbing]                  8\n");
        
    }
    return ;
}
 void* Server::clientThread(void* args_p) {
  
  threadArgs* tmp = (threadArgs*) args_p;
  int clientSock = tmp -> clientSock;
  delete tmp;

  pthread_detach(pthread_self());

  InstantMessage(clientSock);

  close(clientSock);

  pthread_exit(NULL);
}

void Server::InstantMessage(int clientSock) {

  string clientMsg = "";
  long clientMsgLength = 0;
  int messageID = 1;
  fd_set clientfd;
  struct timeval tv;
  int numberOfSocks = 0;
  bool hasRead = true;

  string userName;
  string userpwd;

  while (!hasAuthenticated(clientSock, userName)) {
  }

  broadcastMsg(userName, "", true);

  FD_ZERO(&clientfd);
  tv.tv_sec = 2;
  tv.tv_usec = 100000;

  FD_SET(clientSock, &clientfd);
  numberOfSocks = clientSock + 1;

  while (clientMsg != "/quit" && clientMsg != "/close") {

    string msg = GetMsgs(userName);
    if (msg.length() != 0) {
      if (!SendInteger(clientSock, msg.length()+1)) {
	cerr << "Unable to send Int. " << endl;
	break;
      }

      if (!SendMessage(clientSock, msg)) {
	cerr << "Unable to send Message. " << endl;
	break;
      }
    }
    

    int pollSock = select(numberOfSocks, &clientfd, NULL, NULL, &tv);
    tv.tv_sec = 1;
    tv.tv_usec = 100000;
    FD_SET(clientSock, &clientfd);
    if (pollSock != 0 && pollSock != -1) {
      string tmp;
      long msgLength = GetInteger(clientSock);
      if (msgLength <= 0) {
	cerr << "Couldn't get integer from Client." << endl;
	break;
      }
      string clientMsg = GetMessage(clientSock, msgLength);
      if (clientMsg == "") {
	cerr << "Couldn't get message from Client." << endl;
	break;
      }
      tmp = "Client Said: ";
      tmp.append(clientMsg);
      cout << tmp << endl;
      tmp.clear();
      
      SaveMsg(clientMsg, userName); 
    }
  }//*/

  cout << "Closing Thread." << endl;
  setUserDisconnected (userName);
  broadcastMsg(userName, "", false);
}

void Server::broadcastMsg(string userName, string msg, bool isConnected) {

  if (msg == "") {
    msg = "";
    Msg tmp;
    tmp.to;
    tmp.from = userName;
    tmp.msg = msg;
    tmp.cmd = "/all";

    pthread_mutex_lock(&UserListLock);
    tr1::unordered_map<string, User>::iterator got = UsersList.begin();
    for ( ; got != UsersList.end(); got++) {
      if ((got)->second.isConnected == true && (got)->second.username != userName) {
	tmp.to = (got)->second.username;
	msg.append (userName);
	if (isConnected) {
	  msg.append (" has connected! :)\n");
	} else {
	  msg.append (" has disconnected! :(\n");
	}
	tmp.msg = msg;
	msg.clear();
	addToMsgQueue(tmp);
	tmp.msg.clear();
	
      }
    }
    pthread_mutex_unlock(&UserListLock);
  } else {
    string globMsg = "";
    globMsg.append (userName);
    globMsg.append (" has said: ");
    globMsg.append (msg);
    Msg tmp;
    tmp.from = userName;
    tmp.msg = globMsg;
    tmp.cmd = "/all";
    pthread_mutex_lock(&UserListLock);
    tr1::unordered_map<string, User>::iterator got = UsersList.begin();
    for ( ; got != UsersList.end(); got++) {
      if ((got)->second.isConnected == true && (got)->second.username != userName) {
	tmp.to = (got)->second.username;
	addToMsgQueue(tmp);
	
      }
    }
    pthread_mutex_unlock(&UserListLock);
  }
}


bool Server::hasAuthenticated(int clientSock, string &userName) {

  string loginSuccessMsg = "Login Successful!\n";
  string loginFailureMsg = "Login Failed!\n";
  string userPwd;
  long responseLen = 0;
  string clientResponse;

  responseLen = GetInteger(clientSock);
  userName = GetMessage(clientSock, responseLen);

  responseLen = GetInteger(clientSock);
  userPwd = GetMessage(clientSock, responseLen);
  
  if (loginUser (userName, userPwd)) {
    SendInteger(clientSock, loginSuccessMsg.length()+1);
    SendMessage(clientSock, loginSuccessMsg);
    cout << "Logged in as: " << userName << endl;
    return true;
  } else {
    SendInteger(clientSock, loginFailureMsg.length()+1);
    SendMessage(clientSock, loginFailureMsg);
    cout << "Failed to login as: " << userName << endl;
    return false;
  }
}

bool Server::SendMessage(int HostSock, string msg) {

  int msgLength = msg.length()+1;
  char msgBuff[msgLength];
  strcpy(msgBuff, msg.c_str());
  msgBuff[msgLength-1] = '\0';

  int msgSent = send(HostSock, msgBuff, msgLength, 0);
  if (msgSent != msgLength){
    cerr << "Unable to send data. Closing clientSocket: " << HostSock << "." << endl;
    return false;
  }

  return true;
}

string Server::GetMessage(int HostSock, int messageLength) {

  int bytesLeft = messageLength;
  char buffer[messageLength];
  char* buffPTR = buffer;
  while (bytesLeft > 0){
    int bytesRecv = recv(HostSock, buffPTR, messageLength, 0);
    if (bytesRecv <= 0) {
      cerr << "Could not recv bytes. Closing clientSocket: " << HostSock << "." << endl;
      return "";
    }
    bytesLeft = bytesLeft - bytesRecv;
    buffPTR = buffPTR + bytesRecv;
  }

  return buffer;
}

long Server::GetInteger(int HostSock) {

  int bytesLeft = sizeof(long);
  long networkInt;
  char* bp = (char *) &networkInt;
  
  while (bytesLeft) {
    int bytesRecv = recv(HostSock, bp, bytesLeft, 0);
    if (bytesRecv <= 0){
      cerr << "Failed to receive bytes. Closing clientSocket: " << HostSock << "." << endl;
      return -1;
    }
    bytesLeft = bytesLeft - bytesRecv;
    bp = bp + bytesRecv;
  }
  return ntohl(networkInt);
}

bool Server::SendInteger(int HostSock, int hostInt) {

  long networkInt = htonl(hostInt);

  int didSend = send(HostSock, &networkInt, sizeof(long), 0);
  if (didSend != sizeof(long)){
    cerr << "Unable to send data. Closing clientSocket: " << HostSock << "."  << endl;
    return false;
  }

  return true;
}

void Server::addToMsgQueue(Msg newMsg) {
  pthread_mutex_lock(&MsgQueueLock);
  MsgQueue.push_back(newMsg);
  pthread_mutex_unlock(&MsgQueueLock);
}

string Server::GetMsgs(string username) {
  stringstream ss;
  pthread_mutex_lock(&MsgQueueLock);
  for (int i = 0; i < MsgQueue.size(); i++) {
    if (MsgQueue[i].to == username) {
      if (MsgQueue[i].cmd == "/msg") {
	ss << "/\b\n************************************\npm from " << MsgQueue[i].from << ": ";
	ss << MsgQueue[i].msg << endl << "************************************" << endl;
	MsgQueue.erase (MsgQueue.begin()+i);
	i--;
      } else if (MsgQueue[i].cmd == "/all") {
	ss << MsgQueue[i].msg << endl;
	MsgQueue.erase (MsgQueue.begin()+i);
	i--;
      } else if (MsgQueue[i].cmd == "/users") {
	ss << MsgQueue[i].msg << endl;
	MsgQueue.erase (MsgQueue.begin()+i);
	i--;
      } else if (MsgQueue[i].cmd == "/poke" ) {
	ss << "/\b\n" << MsgQueue[i].from << " has poked you!" << endl;
	MsgQueue.erase (MsgQueue.begin()+i);
	i--;
      } else if (MsgQueue[i].cmd == "/time" ) {
	ss << MsgQueue[i].msg;
	MsgQueue.erase (MsgQueue.begin()+i);
	i--;
      } else if (MsgQueue[i].cmd == "/joke" ) {
	ss << MsgQueue[i].msg;
	MsgQueue.erase (MsgQueue.begin()+i);
	i--;
      } else if (MsgQueue[i].cmd == "/picture" ) {
	ss << MsgQueue[i].msg;
	MsgQueue.erase (MsgQueue.begin()+i);
	i--;
      }
    }
  }
  pthread_mutex_unlock(&MsgQueueLock);
  
  return ss.str();
}

void Server::SaveMsg(string msg, string userFrom) {
  
  Msg newMsg;
  newMsg.msg = msg;
  newMsg.to = "";
  newMsg.from = userFrom;
  newMsg.cmd = "";
  processMsg(newMsg.msg, newMsg.cmd, newMsg.to);

  if (newMsg.cmd == "/all") {
    broadcastMsg(userFrom, msg, false);
  } else if (newMsg.cmd == "/msg") {
      if (doesUserExist(newMsg.to)) {
	addToMsgQueue(newMsg);
      }
  } else if (newMsg.cmd == "/users") {
    newMsg.to = userFrom;
    newMsg.from = "SERVER";
    newMsg.msg = GrabUsers(userFrom);
    addToMsgQueue(newMsg);
  } else if (newMsg.cmd == "/poke") {
      if (doesUserExist(newMsg.to)) {
	addToMsgQueue(newMsg);
      }
  } else if (newMsg.cmd == "/time") {
    newMsg.from = "SERVER";
    if (newMsg.to == "") {
      newMsg.msg = GrabTime(userFrom);
    } else {
      newMsg.msg = GrabTime(newMsg.to);
    }
    newMsg.to = userFrom;
    addToMsgQueue(newMsg);
  } else if (newMsg.cmd == "/joke") {
    newMsg.to = userFrom;
    newMsg.from = "SERVER";
    newMsg.msg = GrabJoke();
    addToMsgQueue(newMsg);
  } else if (newMsg.cmd == "/picture") {
    newMsg.to = userFrom;
    newMsg.from = "SERVER";
    newMsg.msg = GrabPic();
    addToMsgQueue(newMsg);
  }

}

void Server::processMsg(string &msg, string &cmdName, string &userTo) {
  

  if (msg.c_str()[0] == '/') {
    int cmdSize = 0;
    for (int i = 1; i < msg.length(); i++) {
      if (msg.c_str()[i] == ' ') {
	cmdSize = i;
	break;
      }
    }
    if (cmdSize == 0) {
      cmdSize = msg.length();
    }
    for (int i = 0; i < cmdSize; i++) {
      stringstream ss;
      ss << msg.c_str()[i];
      cmdName.append(ss.str());
    }

    int userSize = 0;
    if (cmdName == "/msg" || cmdName == "/poke" || cmdName == "/time") {
      for (int i = cmdSize+1; i < msg.length(); i++) {
	if (msg.c_str()[i] == ' ' ) {
	  userSize = i;
	  break;
	}
      }
      
      if (userSize == 0) {
	userSize = msg.length();
      }

      for (int i = cmdSize+1; i < userSize; i++) {
	stringstream ss;
	ss << msg.c_str()[i];
	userTo.append(ss.str());
      }
    
      msg.replace(0, userSize+cmdSize-3, "");
    } else if (cmdName == "/users" || cmdName == "/joke" || cmdName == "/picture") {
    } 


  } else {
    userTo.append("all");
    cmdName = "/all";
  }
}

string Server::GrabPic() {

  stringstream welcomeMsg;
  welcomeMsg <<"/\b#############################################################"<<endl;
  welcomeMsg <<"#                    _                                      #"<<endl;  
  welcomeMsg <<"#                  -=\\`\\                                    #"<<endl;  
  welcomeMsg <<"#              |\\ ____\\_\\__                                 #"<<endl;  
  welcomeMsg <<"#            -=\\c`\"\"\"\"\"\"\" \"`)                               #"<<endl;  
  welcomeMsg <<"#               `~~~~~/ /~~`                                #"<<endl;  
  welcomeMsg <<"#                 -==/ /                                    #"<<endl;  
  welcomeMsg <<"#                   '-'                                     #"<<endl;  
  welcomeMsg <<"#                  _  _                                     #"<<endl;  
  welcomeMsg <<"#                 ( `   )_                                  #"<<endl;  
  welcomeMsg <<"#                (    )    `)                               #"<<endl;  
  welcomeMsg <<"#              (_   (_ .  _) _)                             #"<<endl;  
  welcomeMsg <<"#                                             _             #"<<endl;  
  welcomeMsg <<"#                                            (  )           #"<<endl;  
  welcomeMsg <<"#             _ .                         ( `  ) . )        #"<<endl;  
  welcomeMsg <<"#           (  _ )_                      (_, _(  ,_)_)      #"<<endl;  
  welcomeMsg <<"#         (_  _(_ ,)                                        #"<<endl; 
  welcomeMsg <<"#############################################################"<<endl;

  return welcomeMsg.str();
}

string Server::GrabJoke() {

  stringstream ss;
  srand (time(NULL));
  int rndNum = rand() % 10 + 1; // between 1 and 10

  switch (rndNum) {
  case 1:
    ss << "/\bMost people believe that if it ain't broke, don't fix it. Engineers believe that if it ain't broke, it doesn't have enough features yet." << endl;
    break;
  case 2:
    ss << "/\bQ: How does a computer tell you it needs more memory?   A: It says ''byte me''" << endl;
    break;
  case 3:
    ss << "/\bQ: What is the first programming language you learn when studying computer science?  A: Profanity" << endl;
    break;
  case 4:
    ss << "/\bA blind man walks into a bar...   and a chair and a table." << endl;
    break;
  case 5:
    ss << "/\bQ: Why don't cows make large bets?   A: The steaks are too high." << endl;
    break;
  case 6:
    ss << "/\bQ: Why aren't jokes in base 8 funny?   A: Because 7, 10, 11." << endl;
    break;
  case 7:
    ss << "/\bQ: What did people say after two satellite dishes got married?   A: The wedding was dull, but the reception was great." << endl;
    break;
  case 8:
    ss << "/\bQ: If Al Gore tried his hand as a musician, what would his album be called?   A. Algorithms." << endl;
    break;
  case 9:
    ss << "/\bA programmer goes to do groceries. His wife tells him: \n-- Buy a loaf of bread, and if they have eggs, buy a dozen.\n";
    ss << " He comes back with thirteen loaves of bread.\n -- 'But why?', she asks.\n --'They had eggs.'" << endl;
    break;
  case 10:
    ss << "/\bSilly chat person, NO JOKE FOR YOU!" << endl;
    break;
  default:
    ss << "/\bSilly chat person, NO JOKE FOR YOU!" << endl;
    break;
  }
  
  return ss.str();
}

string Server::GrabTime(string userName) {

  stringstream ss;

  pthread_mutex_lock(&UserListLock);
  tr1::unordered_map<string, User>::iterator got = UsersList.find (userName);
 if (got == UsersList.end() ) {
    ss << "/\bCould not find: " << userName << endl;
 } else {
   if ((got)->second.isConnected) {
     ss << "/\b" << userName << " has been connected for "
	<< time(NULL) - (got)->second.timeConnected << " seconds." << endl;
   } else {
     ss << "/\b" << userName << " is not connected." << endl;
   }
 }
 pthread_mutex_unlock(&UserListLock);

  return ss.str();
}

string Server::GrabUsers(string userName) {

  stringstream ss;
  int numOfUsers = 1;
  ss << "/\bConnected Users: " << endl;

  pthread_mutex_lock(&UserListLock);
  tr1::unordered_map<string, User>::iterator got = UsersList.begin();
  for ( ; got != UsersList.end(); got++) {
    if ((got)->second.isConnected == true) {
      if ((got)->second.username == userName) {
	ss << numOfUsers++ << ". " << "You" << endl;
      } else {
	ss << numOfUsers++ << ". " << (got)->second.username << endl;
      }
    }
  }
  pthread_mutex_unlock(&UserListLock);

  return ss.str();
}

bool Server::loginUser (string username, string password) {
  User newUser;
  newUser.username = username;
  newUser.password = password;
  newUser.isConnected = true;
  newUser.timeConnected = time(NULL);
  pthread_mutex_lock(&UserListLock);
  tr1::unordered_map<string, User>::iterator got = UsersList.find (username);
  if (got == UsersList.end() ) {
    UsersList.insert (make_pair<string, User> (newUser.username, newUser));
    pthread_mutex_unlock(&UserListLock);
    return true;
  } else {
    if (got->second.password == password) {
      if (got->second.isConnected) {
	pthread_mutex_unlock(&UserListLock);
	return false;
      } else {
	got->second.isConnected = true;
	got->second.timeConnected = time(NULL);
	pthread_mutex_unlock(&UserListLock);
	return true;
      }
    } else {
      pthread_mutex_unlock(&UserListLock);
      return false;
    }
  }
}

void Server::addToUsersList (User newUser) {
  pthread_mutex_lock(&UserListLock);
  UsersList.insert (make_pair<string, User> (newUser.username, newUser));
  pthread_mutex_unlock(&UserListLock);
}

bool Server::doesUserExist (string username) {
  pthread_mutex_lock(&UserListLock);
  tr1::unordered_map<string, User>::const_iterator got = UsersList.find (username);
  if (got == UsersList.end() ) {
    pthread_mutex_unlock(&UserListLock);
    return false;
  } else {
    pthread_mutex_unlock(&UserListLock);
    return true;
  }
  
}

bool Server::isUserConnected (User newUser) {
  pthread_mutex_lock(&UserListLock);
  tr1::unordered_map<string, User>::const_iterator got = UsersList.find (newUser.username);
  if (got == UsersList.end() ) {
    pthread_mutex_unlock(&UserListLock);
    return false;
  } else {
    if (got->second.isConnected) {
      pthread_mutex_unlock(&UserListLock);
      return true;
    } else {
      pthread_mutex_unlock(&UserListLock);
      return false;
    }
  }
}

void Server::setUserConnected (User newUser) {
  pthread_mutex_lock(&UserListLock);
  tr1::unordered_map<string, User>::iterator got = UsersList.find (newUser.username);
  if (got == UsersList.end() ) {
    pthread_mutex_unlock(&UserListLock);
  } else {
    got->second.isConnected = true;
    got->second.timeConnected = time(NULL);
    pthread_mutex_unlock(&UserListLock);
  }
}

void Server::setUserDisconnected (string username) {
  pthread_mutex_lock(&UserListLock);
  tr1::unordered_map<string, User>::iterator got = UsersList.find (username);
  if (got == UsersList.end() ) {
    pthread_mutex_unlock(&UserListLock);
  } else {
    got->second.isConnected = false;
    pthread_mutex_unlock(&UserListLock);
  }
}
