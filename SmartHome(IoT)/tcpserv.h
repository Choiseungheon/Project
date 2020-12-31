#ifndef TCPSERV_H
#define TCPSERV_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 1024
#define VOICE "192.168.0.3"
#define DU "192.168.0.31"
#define SH "192.168.0.30"

void csh_SndUDPData(char* ip,  int port, int);
void kdu_SndUDPData(char* ip,  int port, int);
void error_handling(char *message);
void* client_main(void *data);

#endif
