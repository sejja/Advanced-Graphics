#ifndef __REMOTESERVER__H__
#define __REMOTESERVER__H__


#include <Dependencies/Json/single_include/json.hpp>

using json = nlohmann::json;


class Server{

public:

	void StartServer();
	void KillServer();


	void broadcastMessage(const json& message);

private:

	void CreateServer();
	void DestroyServer();

};



#endif 