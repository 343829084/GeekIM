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

const int INPUT_LINES = 3;
const char ENTER_SYM = '\n';
const char BACKSPACE_SYM = '\b';
const int DELETE_SYM = 127;
WINDOW *INPUT_SCREEN;
WINDOW *MSG_SCREEN;
fd_set hostfd;
struct timeval tv;
pthread_mutex_t displayLock;
int displayStatus = pthread_mutex_init(&displayLock, NULL);
string serverRsp;



int main (int argNum, char* argValues[]) {

  string inputStr;
  string hostname;
  string username = "";
  unsigned short serverPort;
  int hostSock;
  int numberOfSocks;

  if (argNum < 3 || argNum > 3){
    cerr << "Incorrect number of arguments. Please try again." << endl;
    return -1;
  }

  hostname = argValues[1];
  serverPort = atoi(argValues[2]);

  prepareWindows();

  hostSock = openSocket(hostname, serverPort);
  
  while (!hasAuthenticated(hostSock, username)) {
  }
  string welcomeMsg = "\nWelcome!\n\n";
  displayMsg(welcomeMsg);
  wrefresh(INPUT_SCREEN);
  
  struct threadArgs* args_p = new threadArgs;
  args_p -> clientSock = hostSock;
  pthread_t tid;
  int threadStatus = pthread_create(&tid, NULL, clientThread, (void*)args_p);
  if (threadStatus != 0){
    cerr << "Failed to create child process." << endl;
  }

  if (hostSock > 0 ) {
    while (true) {

      if (getUserInput(inputStr, false)) {

	if (inputStr == "/quit" || inputStr == "/exit" || inputStr == "/close") {
	  if (!SendInteger(hostSock, inputStr.length()+1)) {
	    cerr << "Unable to send Int. " << endl;
	    break;
	  }
	  if (!SendMessage(hostSock, inputStr)) {
	    cerr << "Unable to send Message. " << endl;
	    break;
	  }
	  break;
	}

	string tmp = "You said: ";
	tmp.append(inputStr);
	tmp.append("\n");
	displayMsg(tmp);

	if (!SendInteger(hostSock, inputStr.length()+1)) {
	  cerr << "Unable to send Int. " << endl;
	  break;
	}

	if (!SendMessage(hostSock, inputStr)) {
	  cerr << "Unable to send Message. " << endl;
	  break;
	}

	inputStr.clear();
	clearInputScreen();
      }
    }
  }

  delwin(INPUT_SCREEN);
  delwin(MSG_SCREEN);
  endwin();
  close(hostSock);

  exit(-1);
}

