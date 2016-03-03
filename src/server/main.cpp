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

const int MAXPENDING = 20;
tr1::unordered_map<string, User> UsersList;
deque<Msg> MsgQueue;
pthread_mutex_t MsgQueueLock;
pthread_mutex_t UserListLock;
int MsgStatus = pthread_mutex_init(&MsgQueueLock, NULL);
int UserStatus = pthread_mutex_init(&UserListLock, NULL);


int main(int argc, char* argv[]){

  unsigned short serverPort; 
  if (argc != 2){
    cerr << "Incorrect number of arguments. Please try again." << endl;
    return -1;
  }
  serverPort = atoi(argv[1]);

  int conn_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (conn_socket < 0){
    cerr << "Error with socket." << endl;
    exit(-1);
  }

  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;    
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(serverPort);
  
  int sock_status = bind(conn_socket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
  if (sock_status < 0) {
    cerr << "Error with bind." << endl;
    exit(-1);
  }

  int listen_status = listen(conn_socket, MAXPENDING);
  if (listen_status < 0) {
    cerr << "Error with listening." << endl;
    exit(-1);
  }
  cout << endl << endl << "SERVER: Ready to accept connections. " << endl;


  while (true) {
    struct sockaddr_in clientAddress;
    socklen_t addrLen = sizeof(clientAddress);
    int clientSocket = accept(conn_socket, (struct sockaddr*) &clientAddress, &addrLen);
    if (clientSocket < 0) {
      cerr << "Error accepting connections." << endl;
      exit(-1);
    }

    struct threadArgs* args_p = new threadArgs;
    args_p -> clientSock = clientSocket;
    pthread_t tid;
    int threadStatus = pthread_create(&tid, NULL, clientThread, (void*)args_p);
    if (threadStatus != 0){
      cerr << "Failed to create child process." << endl;
      close(clientSocket);
      pthread_exit(NULL);
    }
    
  }

  return 0;
}

