#ifndef CLIENTWINDOWS_H
#define CLIENTWINDOWS_H
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define MAX 1024*256
#define PORT 50050
#define SA struct sockaddr

char* changeExtension(char* myStr);
extern int connectToServer(const char* filePath);
extern void sendFile(SOCKET sockfd, const char* filePath);
extern void receiveModifiedFile(SOCKET sockfd, const char filePath[]);
#endif // CLIENTWINDOWS_H
