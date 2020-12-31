// gcc -o ser thread_01.c  xxx.c -lpthread
#include "tcpserv.h"

void* client_main(void *data)
{
  //init
  int clnt_sock = *((int*)data);
  int recvByte=0;
  int sendByte=0;
  char type= -1;
  char cmd = -1;
  char typeArr[2];

  char recvBuf[MAX_SIZE];
  int readByte = -1;
  int writeByte=-1;
  
  readByte = read(clnt_sock, typeArr, sizeof(typeArr));
  printf("client : atoi(%d) || atoi(%d) || readByte[%d]\n", atoi(&typeArr[0]),atoi(typeArr) ,readByte);

  writeByte = write(clnt_sock, &writeByte, sizeof(10));
  printf("write type : %d\n",writeByte);

  //light on 1 / off 2 // fan on 3 / off 4
  switch (atoi(typeArr))
  {
    case 1:
        csh_SndUDPData(SH,5000,1);
        printf("csh_light : on\n");
        break;
    case 2:
        csh_SndUDPData(SH,5000,2);
        printf("csh_light : off\n");
        break;
    case 3:
        csh_SndUDPData(SH,5000,3);
        printf("csh_fan : on\n");
        break;
    case 4:
        csh_SndUDPData(SH,5000,4);
        printf("csh_fan : off\n");
        break;
    case 5://김동욱
        kdu_SndUDPData(DU,5000,5);
        printf("KDU_LED : Romantic\n");
        break;
    case 6:
        kdu_SndUDPData(DU,5000,7);
        printf("KDU_LED : Warm\n");
        break;
    case 7:
        kdu_SndUDPData(DU,5000,6);
        printf("KDU_LED : Funny\n");
        break;
    case 8:
        kdu_SndUDPData(DU,5000,8);
        printf("KDU_LED : PIR on\n");
        break;    
    default:
        break;
  }

  printf("\nclient sock [%d] exit....\n",clnt_sock);
  close(clnt_sock);

}
  