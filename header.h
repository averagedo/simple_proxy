#include<iostream>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include<string>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>

using namespace std;

void listenREQ(int port);

void executeREQ(string data, int newsockfd);

void copydata(char *&mainB, int &lenmain, char *subB, int lensub);

void sigusr1_fun(int signum);

void sigusr2_fun(int signu);


