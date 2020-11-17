#include "header.h"

void sigusr1_fun(int signu)
{

}

void sigusr2_fun(int signum)
{
  cout<<"EXIT PROXY"<<endl;
  exit(9);
}

void listenREQ(int port)
{
  char buff[5000] = {0};
  int sockfd, newsockfd, opt = 1;
  int datalen;
  sockaddr_in addr;
  int addrlen = sizeof(addr);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    cout << "socket error\n";
    exit(1);
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    cout << "setsockopt error\n";
    exit(1);
  }
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0)
  {
    cout << "bind error\n";
    exit(1);
  }

  if (listen(sockfd, SOMAXCONN) < 0)
  {
    cout << "listen error\n";
    exit(1);
  }

  while (1)
  {
    memset(buff, 0, sizeof(buff));
    if ((newsockfd = accept(sockfd, (sockaddr *)&addr, (socklen_t *)&addrlen)) < 0)
    {
      cout << "accept error\n";
      exit(1);
    }

    if ((datalen = recv(newsockfd, buff, sizeof(buff), 0)) < 0)
    {
      cout << "recv error\n";
      exit(1);
    }

    string data = buff;
    pid_t child_pid = fork();
    if (child_pid == 0)
    {
      close(sockfd);
      executeREQ(data, newsockfd);
      cout << "da huy" << endl;
      close(newsockfd);
      _exit(0);
    }
    else if (child_pid > 0)
    {
      signal(SIGCHLD, SIG_IGN);
      close(newsockfd);
    }
    else
    {
      cout << "fork error\n";
      //exit(1);
    }
  }
}

void executeREQ(string data, int newsockfd)
{
  int indexmethod=data.find(" ");
  string method=data.substr(0,indexmethod);
  cout<<method<<endl;

  if(method.compare("HEAD")==0 && method.compare("GET")==0)
  {
    string http405="HTTP/1.1 405 Method Not Allowed\r\n\r\n";
    send(newsockfd,http405.c_str(),http405.length(),0);
    close(newsockfd);
    return;
  }

  int index = data.find("Host:");
  if(index==string::npos)
  {
    cout<<"Find host fail\n";
    return;
  }
  index += sizeof("Host: ") - 1;
  int secondR = data.find('\r');
  secondR = data.find('\r', secondR + 1);
  string domain = data.substr(index, secondR - index);

  cout << domain << endl;

  hostent *host = gethostbyname(domain.c_str());
  //cout<<inet_ntoa(*(in_addr*)(host->h_addr_list[0]))<<endl;

  //========================================================
  //================== SEND and RECV =======================
  //========================================================
  int sockfd_1;
  sockaddr_in addr_1;
  if ((sockfd_1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    cout << "socket error\n";
    exit(1);
  }

  addr_1.sin_family = AF_INET;
  addr_1.sin_port = htons(80);
  addr_1.sin_addr = *(in_addr *)host->h_addr_list[0];

  if (connect(sockfd_1, (sockaddr *)&addr_1, sizeof(addr_1)) < 0)
  {
    cout << "connect error\n";
    fprintf(stderr, "%m\n");
    exit(1);
  }

  int countsend;
  if ((countsend = send(sockfd_1, data.c_str(), data.length(), 0)) < 0)
  {
    cout << "send error\n";
    exit(1);
  }

  if(shutdown(sockfd_1,SHUT_WR)<1)
  {
    cout<<"shutdown error\n";
  }
  //=====

  char buff_1[10000];
  int countrecv;
  char *mainbuff;
  int lenmain;
  while (1)
  {
    memset(buff_1, 0, sizeof(buff_1));
    countrecv = recv(sockfd_1, buff_1, sizeof(buff_1), 0);
    if (countrecv < 0)
    {
      cout << "recv error\n";
      exit(1);
    }
    
    //cout<<buff_1;

    if (countrecv == 0)
      break;

    copydata(mainbuff, lenmain, buff_1, countrecv);

    cout << "==> " << countrecv << endl;

    //if (countrecv < sizeof(buff_1))
      //break;
  }

  cout << "==> " << lenmain << endl;
  cout << mainbuff << endl;

  //====
  if ((countsend = send(newsockfd, mainbuff, lenmain, 0)) < 0)
  {
    cout << "send error\n";
    exit(1);
  }

  delete[] mainbuff;

  close(newsockfd);
  close(sockfd_1);
}

void copydata(char *&mainB, int &lenmain, char *subB, int lensub)
{
  if (lenmain == 0)
  {
    mainB = new char[lensub];
    memcpy(mainB, subB, lensub);
    lenmain = lensub;
  }
  else
  {
    char *term = new char[lenmain];
    memcpy(term, mainB, lenmain);
    delete[] mainB;
    mainB = new char[lenmain + lensub];
    memcpy(mainB, term, lenmain);
    delete[] term;
    for (int i = 0; i < lensub; i++)
    {
      mainB[i + lenmain] = subB[i];
    }
    lenmain += lensub;
  }
}