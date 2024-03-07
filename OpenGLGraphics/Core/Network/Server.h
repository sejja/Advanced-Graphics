#ifndef __NETWORKSERVER__H__
#define __NETWORKSERVER__H__


#include <winsock2.h>
#include <WS2tcpip.h>
#include <Dependencies/Json/single_include/json.hpp>
#include <windows.h>
#include "Core/ECSystem/Object.h"

using json = nlohmann::json;


class Server{

public:

	int CreateServer();
	int StartServer();
	void KillServer();

	void sendObjectIfChanged(const std::shared_ptr<Core::Object>& obj);

	bool isRunning() {
		return serverSocket != NULL;
	}


private:
	WSADATA WSAData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddr, clientAddr;
	int serverPort = 5555;
	std::shared_ptr<Core::Object> lastSentObject;

	void sendToClient(const json& message);

	


};



#endif 