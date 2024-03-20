#pragma once
#ifndef SERVERWINDOWS_H
#define SERVERWINDOWS_H

#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <locale.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX 1024*256
#define PORT 50050

void receiveFile(SOCKET connfd);
void modifyAndSendFile(SOCKET connfd);

#endif 
