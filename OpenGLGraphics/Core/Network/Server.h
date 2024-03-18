#ifndef __NETWORKSERVER__H__
#define __NETWORKSERVER__H__


#include <winsock2.h>
#include <WS2tcpip.h>
#include <Dependencies/Json/single_include/json.hpp>
#include <windows.h>
#include "Core/ECSystem/Object.h"
#include "Common.h"

using json = nlohmann::json;


class Server: public Common {
	
public:

	int CreateServer();
	int StartServer();
	void KillServer();

	void BroadcastServerPresence();


	bool isRunning() {
		return serverSocket != NULL;
	}


private:
	WSADATA WSAData;
	SOCKADDR_IN serverAddr, clientAddr;
	int serverPort = 5555;
	

};



#endif 