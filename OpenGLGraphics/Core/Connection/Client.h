#ifndef __REMOTECLIENT__H__
#define __REMOTECLIENT__H__

#include <Dependencies/Json/single_include/json.hpp>
#include <windows.h>


using json = nlohmann::json;

class Client {

public:

	void connectToServer(const char* ip, int port);
	void disconnectFromServer();
	void sendToServer(const json& message);



private:
	char* serverIP;
	int serverPort;
	int clientSocket;
	bool connected;

	DWORD WINAPI receiveFromServer(LPVOID lpParam);

	static DWORD WINAPI ThreadStartRoutine(LPVOID lpParam);

};



#endif 