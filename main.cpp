#include"header.h"

int main(int argc,char* argv[])
{
  if(argc!=2)
  {
    cout<<"./12843 <port>\n";
    return 1;
  }

  int port=stoi(argv[1]);
  signal(SIGINT,SIG_IGN);
  signal(SIGUSR1,sigusr1_fun);
  signal(SIGUSR2,sigusr2_fun);
  listenREQ(port);

  return 0;

}