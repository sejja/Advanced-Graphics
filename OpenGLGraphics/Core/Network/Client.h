#ifndef __NETWORKCLIENT__H__
#define __NETWORKCLIENT__H__

#include <winsock2.h>
#include <WS2tcpip.h>
#include <Dependencies/Json/single_include/json.hpp>
#include <windows.h>
#include "Common.h"


using json = nlohmann::json;

class Client : public Common {

public:

	void connectToServer(const char* ip, int port);
	void disconnectFromServer();
	
	bool isConnected() const { return clientSocket != INVALID_SOCKET; }




private:
	char* serverIP;
	int serverPort;

	//DWORD WINAPI receiveFromServer(LPVOID lpParam);



};



#endif 