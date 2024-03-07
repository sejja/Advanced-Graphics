#ifndef __NETWORKSERVER__H__
#define __NETWORKSERVER__H__


#include <winsock2.h>
#include <WS2tcpip.h>
#include <Dependencies/Json/single_include/json.hpp>
#include <windows.h>

using json = nlohmann::json;


class Server{

public:

	int StartServer();
	void KillServer();


	void sendToClient(const json& message);

private:
	WSADATA WSAData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddr, clientAddr;
	int serverPort = 5555;

};



#endif 