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
	
	bool isConnected() const { return clientSocket != NULL; }

	void findServers(int port);

	void ListenForServers();

	std::vector<std::string> getServers() const { return availableServers; }

	bool getIsBroadcastBinded() const { return isBroadcastBinded; }

	void closeBroadcastSocket();

private:
	char* serverIP;
	int serverPort;
	std::vector<std::string> availableServers;

	bool isBroadcastBinded = false;

	SOCKET broadcastSocket;





};



#endif 