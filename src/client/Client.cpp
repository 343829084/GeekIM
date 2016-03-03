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
#include"Common.h"
#include"Client.h"

using namespace std;



bool Client::hasAuthenticated (int hostSock, string &username) {

  // Locals
  string loginMsg = "////////////////////////////////////////////////////////\nPlease enter your username.\nThe system will create a new account if your username could not be found.\n";
  string pwdMsg = "/\b\nPlease enter your password.\n";
  string clearScr = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
  string userName;
  string userPwd;
  long responseLen = 0;
  string hostResponse;

  displayMsg(clearScr);
  wrefresh(INPUT_SCREEN);
  clearInputScreen();

  displayMsg(loginMsg);
  wrefresh(INPUT_SCREEN);
  while (!getUserInput(userName, false)) {
  }
  clearInputScreen();

  displayMsg(pwdMsg);
  wrefresh(INPUT_SCREEN);
  while (!getUserInput(userPwd, true)) {
  }
  clearInputScreen();

  displayMsg(clearScr);
  wrefresh(INPUT_SCREEN);
  clearInputScreen();
  
  SendInteger(hostSock, userName.length()+1);
  SendMessage(hostSock, userName);
  SendInteger(hostSock, userPwd.length()+1);
  SendMessage(hostSock, userPwd);

  responseLen = GetInteger(hostSock);
  hostResponse = GetMessage(hostSock, responseLen);

  if (hostResponse == "Login Successful!\n") {
    username = userName;
    return true;
  } else {
    return false;
    
  }

}


bool Client::SendMessage(int HostSock, string msg) {

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

string Client::GetMessage(int HostSock, int messageLength) {

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

long Client::GetInteger(int HostSock) {

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

bool Client::SendInteger(int HostSock, int hostInt) {

  long networkInt = htonl(hostInt);

  int didSend = send(HostSock, &networkInt, sizeof(long), 0);
  if (didSend != sizeof(long)){
    cerr << "Unable to send data. Closing clientSocket: " << HostSock << "."  << endl;
    return false;
  }

  return true;
}

int Client::openSocket (string hostName, unsigned short serverPort) {

  struct hostent* host;
  int status;
  int bytesRecv;

  int hostSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (hostSock <= 0) {
    cerr << "Socket was unable to be opened." << endl;
    return -1;
  }

  host = gethostbyname(hostName.c_str());
  if (!host) {
    cerr << "Unable to resolve hostname's ip address. Exiting..." << endl;
    return -1;
  }
  char* tmpIP = inet_ntoa( *(struct in_addr *)host->h_addr_list[0]);
  unsigned long serverIP;
  status = inet_pton(AF_INET, tmpIP,(void*) &serverIP);
  if (status <= 0) return -1;

  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = serverIP ;
  serverAddress.sin_port = htons(serverPort);

  status = connect(hostSock, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
  if (status < 0) {
    cerr << "Error with the connection." << endl;
    return -1;
  }

  return hostSock;
}

void Client::prepareWindows() {

  initscr();
  start_color();

  halfdelay(1);

  noecho();
  cbreak();
  MSG_SCREEN = newwin(LINES - INPUT_LINES, COLS, 0, 0);
  scrollok(MSG_SCREEN, TRUE);
  wsetscrreg(MSG_SCREEN, 0, LINES - INPUT_LINES - 1);
  wrefresh(MSG_SCREEN);

  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  wbkgd(MSG_SCREEN, COLOR_PAIR(1));

  INPUT_SCREEN = newwin(INPUT_LINES, COLS, LINES - INPUT_LINES, 0);
  
  clearInputScreen();
  
}

void Client::clearInputScreen() {

  werase(INPUT_SCREEN);
  mvwhline(INPUT_SCREEN, 0, 0, '-', COLS);

  wmove(INPUT_SCREEN, 1, 0);
  waddstr(INPUT_SCREEN, "Input:  ");

  wrefresh(INPUT_SCREEN);

}

bool Client::getUserInput(string& inputStr, bool isPwd) {

  bool success = false;
  int userText;

  userText = wgetch(INPUT_SCREEN);

  if (userText == ERR)
    return false;

  if (isprint(userText)) {
    inputStr += (char)userText;
    if (!isPwd) {
      waddch(INPUT_SCREEN, userText);
    } else {
      char COVER = '*';
      waddch(INPUT_SCREEN, COVER);
    }
  }
  else {
    if (userText == BACKSPACE_SYM || userText == DELETE_SYM) {
      if (inputStr.length() > 0) {
	inputStr.replace(inputStr.length()-1, 1, "");
	wmove(INPUT_SCREEN, 1, 8 + inputStr.length());
	userText = 32;
	waddch(INPUT_SCREEN, userText);
	wmove(INPUT_SCREEN, 1, 8 + inputStr.length());
      }

      wrefresh(INPUT_SCREEN);
    }
    
    if (userText == ENTER_SYM) {
      if (inputStr.size() > 0) {
	success = true;
      }
    }
  }
  return success;
}

void Client::displayMsg(string &msg) {
  pthread_mutex_lock(&displayLock);
  if (msg.c_str()[0] == '/') {
    string blank = "\n";
    waddstr(MSG_SCREEN, blank.c_str());
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    wbkgd(MSG_SCREEN, COLOR_PAIR(2));
    
  } else {
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    wbkgd(MSG_SCREEN, COLOR_PAIR(1));
  }
  waddstr(MSG_SCREEN, msg.c_str());
  
  wrefresh(MSG_SCREEN);
  pthread_mutex_unlock(&displayLock);

}


void* Client::clientThread(void* args_p) {
  
  threadArgs* tmp = (threadArgs*) args_p;
  int hostSock = tmp -> clientSock;
  delete tmp;

  pthread_detach(pthread_self());

  DisplayData(hostSock);

  close(hostSock);

  pthread_exit(NULL);
}

void Client::DisplayData (int hostSock) {

  bool canRead = true;
  fd_set hostfd;
  struct timeval tv;

  FD_ZERO(&hostfd);
  tv.tv_sec = 1;
  tv.tv_usec = 10000;

  FD_SET(hostSock, &hostfd);
  int numberOfSocks = hostSock + 1;

  while (canRead) {
    int pollSock = select(numberOfSocks, &hostfd, NULL, NULL, &tv);
    tv.tv_sec = 1;
    tv.tv_usec = 10000;
    FD_SET(hostSock, &hostfd);
    if (pollSock != 0 && pollSock != -1) {
      long msgLength = GetInteger(hostSock);
      if (msgLength <= 0) {
	cerr << "Couldn't get integer from Client." << endl;
	break;
      }
      string clientMsg = GetMessage(hostSock, msgLength);
      if (clientMsg == "") {
	cerr << "Couldn't get message from Client." << endl;
	break;
      }
      displayMsg(clientMsg);
      wrefresh(INPUT_SCREEN);
    }
  }
}
