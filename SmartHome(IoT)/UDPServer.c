#include <stdio.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <string.h>

#include <stdlib.h>

#include <wiringPi.h>

#include <softPwm.h> 

 

#define SERVO_ON 18 //18   1

#define SERVO_OFF 23 // 23  4

#define FAN 24 // GPIO PIN

#define MAX_SIZE 1024

 

int main(int argc, char*argv[])

{

  int serv_sock;

  struct sockaddr_in serv_addr={0,};

  struct sockaddr_in client_addr={0,};

 

  wiringPiSetupGpio();

  if (wiringPiSetup() == -1)

      return 1;

  pinMode(FAN, OUTPUT);

 

  softPwmCreate(SERVO_ON, 0, 200);

  softPwmCreate(SERVO_OFF, 0, 200);

 

  //socket  UDP,IPV4

  serv_sock = socket(PF_INET, SOCK_DGRAM, 0 );

 


  serv_addr.sin_family=AF_INET;

  serv_addr.sin_addr.s_addr= htonl(INADDR_ANY);

  serv_addr.sin_port = htons(5000);

  bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

 

  //client 통신

  char recvBuf[MAX_SIZE];

  socklen_t client_size = sizeof(client_addr);

  int recv = 0;

  char sel;

 

  while(1){


 

      recvfrom(serv_sock,&recv,sizeof(recv), 0,

          (struct sockaddr*)&client_addr, &client_size);

 

      printf(" Order => %d \n", recv);

   

 

      if (recv == 1) {

          printf("light ON ! \n");

          softPwmWrite(SERVO_ON, 5);

          sleep(3);

          softPwmWrite(SERVO_ON, 24);  // gpio 18

 

    

     

      }

      if (recv == 2) {

          printf("light OFF ! \n");

          softPwmWrite(SERVO_OFF, 24);

          sleep(3);

          softPwmWrite(SERVO_OFF, 5);  // gpio 23

 

   

      }

      if (recv == 3) {

          printf("FAN ON ! \n");

          digitalWrite(FAN, 1);

          

      }

      if (recv == 4) {

          printf("FAN OFF ! \n");       

          digitalWrite(FAN, 0);       

      }

 

  

 

  }

  //serv_socket 닫기

  close(serv_sock);

 

  return 0;

}