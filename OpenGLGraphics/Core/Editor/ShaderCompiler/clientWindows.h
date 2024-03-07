#ifndef CLIENTWINDOWS_H
#define CLIENTWINDOWS_H

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define MAX 1024*256
#define PORT 50050
#define SA struct sockaddr

void patata();
void sendFile(SOCKET sockfd);
void receiveModifiedFile(SOCKET sockfd);

#endif 
