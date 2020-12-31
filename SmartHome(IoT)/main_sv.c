//gcc -o serv main_sv.c send.c thread.c -lpthread 
#include "tcpserv.h"

int main(int argc, char*argv[])
{
  //init
  int serv_sock,clnt_sock;
  struct sockaddr_in serv_addr={0,};
  struct sockaddr_in client_addr={0,};
  socklen_t clnt_addr_size= sizeof(client_addr);

  int v=0;
  int recvByte=0;
  int sendByte=0;
  char type= -1;
  char cmd = -1;
  char recvBuf[MAX_SIZE];
  int readByte = -1;
  int thr_value;
  pthread_t t_id;

  //serv set
  {
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    printf("server sock -- [%d] \n\n",serv_sock);
    if(serv_sock < 0)
      error_handling("socket() error"); 

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr= htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
      error_handling("bind() error");
    if(listen(serv_sock, 5)==-1)
      error_handling("listen() error");
  }

  while(1)
  {     
    clnt_sock = accept(serv_sock,(struct sockaddr*)&client_addr,&clnt_addr_size);

    if( clnt_sock < 0 )
    {
      printf("...clnt_sock [%d] : %s \n",clnt_sock);
    }
    printf("connect succeed - [%s]\n",inet_ntoa(client_addr.sin_addr));
    thr_value = pthread_create(&t_id, NULL, client_main, (void *)&clnt_sock);
    if (thr_value < 0)
    {
      perror("thread create error : ");
      exit(0);
    }

  }

}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}