#ifndef __NETWORKCOMMON__H__
#define __NETWORKCOMMON__H__


#include <winsock2.h>
#include <WS2tcpip.h>
#include <Dependencies/Json/single_include/json.hpp>
#include <windows.h>
#include "Core/ECSystem/Object.h"
#include "Core/ParticleSystem/ParticleSystem.h"
#include "Core/ParticleSystem/FireSystem.h"
#include "Graphics/Primitives/Decal.h"



using json = nlohmann::json;

class Common {
public:

    void sendToPeer(const json& message);
    void sendObjectIfChanged(const std::shared_ptr<Core::Object>& obj);
    void sendParticleIfChanged(const std::shared_ptr<Core::Particles::FireSystem>& fireSystem);

protected:
    SOCKET serverSocket, clientSocket;
    std::shared_ptr<Core::Object> lastSentObject;
    std::shared_ptr<Core::Particles::ParticleSystem> lastSentParticleSys;
    

    static DWORD WINAPI ReceiveThread(LPVOID lpParam);

    static void transformObject(const json& receivedJson);

    static void transformParticle(const json& data);

};


#endif