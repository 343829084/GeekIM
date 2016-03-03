#include<iostream>
#include<stdlib.h>
#include"Server.h"

using namespace std;



int main(int argc, char* argv[]){

  int serverPort; 
  if (argc != 2){
    cerr << "Incorrect number of arguments. Please try again." << endl;
    return -1;
  }
  serverPort = atoi(argv[1]);
  Server im_server(serverPort);
  im_server.Start();
  return 0;

}

