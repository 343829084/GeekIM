#include<iostream>
#include<stdlib.h>
#include"Server.h"

using namespace std;

static Server* server = NULL;

int main(int argc, char* argv[]){

  unsigned short serverPort; 
  if (argc != 2){
    cerr << "Incorrect number of arguments. Please try again." << endl;
    return -1;
  }
  serverPort = atoi(argv[1]);
  server = new Server(serverPort);
  server->Start();

#ifdef _wbing
  Server im_server(serverPort);
  im_server.Start();
#endif
  return 0;

}

