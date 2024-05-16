#ifndef __NETMANAGER__H__
#define __NETMANAGER__H__

#include "Common.h"
#include "Server.h"
#include "Client.h"



class NetManager {

	

public:

	bool isClient() {
        if (server.isRunning()) {
			return false;
        }
        else if (client.isConnected()) {
			return true;
        }
	}

	bool isServer() {
		if (server.isRunning()) {
			return true;
		}
		else if (client.isConnected()) {
			return false;
		}
	}



	Server& GetServer() {
		return server;
	}

	Client& GetClient() {
		return client;
	}


	/*
	template<typename T>
	T& getInstance() {
		if (isServer()) {
			return GetServer();
		}
		else {
			return GetClient();
		}
	}
	*/
	

	



private:
	//Todo usar estos en vez de un singleton de cada y un singleton a manager
	//Server& server;
	//Client& client;

	Server& server = Singleton<Server>::Instance();
	Client& client = Singleton<Client>::Instance();



};

#endif