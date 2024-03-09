#ifndef __NETWORKCOMMON__H__
#define __NETWORKCOMMON__H__


#include <winsock2.h>
#include <WS2tcpip.h>
#include <Dependencies/Json/single_include/json.hpp>
#include <windows.h>
#include "Core/ECSystem/Object.h"



using json = nlohmann::json;

class Common {
public:

    void sendToPeer(const json& message);
    void sendObjectIfChanged(const std::shared_ptr<Core::Object>& obj);


protected:
    SOCKET serverSocket, clientSocket;
    std::shared_ptr<Core::Object> lastSentObject;

    static DWORD WINAPI ReceiveThread(LPVOID lpParam);

    static void transformObject(const json& receivedJson);

};


#endif