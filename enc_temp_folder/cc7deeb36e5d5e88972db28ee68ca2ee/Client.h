#ifndef __REMOTECLIENT__H__
#define __REMOTECLIENT__H__

#include <winsock2.h>
#include <WS2tcpip.h>
#include <Dependencies/Json/single_include/json.hpp>
#include <windows.h>


using json = nlohmann::json;

class Client {

public:

	void connectToServer(const char* ip, int port);
	void disconnectFromServer();
	void sendToServer(const json& message);

	bool isConnected() const { return clientSocket != INVALID_SOCKET; }

	



private:
	char* serverIP;
	int serverPort;
	//int clientSocket
	SOCKET clientSocket;

	DWORD WINAPI receiveFromServer(LPVOID lpParam);

	//DWORD WINAPI ReceiverThread(LPVOID lpParam);


};



#endif 