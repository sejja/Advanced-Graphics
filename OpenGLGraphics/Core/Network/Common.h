#ifndef __NETWORKCOMMON__H__
#define __NETWORKCOMMON__H__

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include "Core/ECSystem/Object.h"


class Server; 
class Client;

class Common {
public:
    static void sendObjectIfChanged(Server& server, const std::shared_ptr<Core::Object>& obj);
   // DWORD WINAPI ReceiverThread(LPVOID lpParam);
};


#endif