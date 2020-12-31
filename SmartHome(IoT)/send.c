#include "tcpserv.h"

//UDF
//light on 1 / off 2 // fan on 3 / off 4
void csh_SndUDPData(char* ip,  int port, int send)
{
  int sock;
  struct sockaddr_in serv_addr={0,};
  char sendBuf[MAX_SIZE];
  char recvBuff[MAX_SIZE];
 // socklen_t addr_size = sizeof(client_addr);

  sock = socket(PF_INET,SOCK_DGRAM,0);  

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(ip);
  serv_addr.sin_port = htons(port);

  sendto(sock,&send, sizeof(send), 0,
                 (struct sockaddr*)&serv_addr,sizeof(serv_addr) );

  close(sock);
}

void kdu_SndUDPData(char* ip,  int port, int send)
{
  int sock;
  struct sockaddr_in serv_addr={0,};
  char recvBuff[MAX_SIZE];
 // socklen_t addr_size = sizeof(client_addr);
  
  sock = socket(PF_INET,SOCK_DGRAM,0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(ip);
  serv_addr.sin_port = htons(port);

  sendto(sock,&send, sizeof(send), 0,
                 (struct sockaddr*)&serv_addr,sizeof(serv_addr) );

  close(sock);
}