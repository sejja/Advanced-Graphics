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
#include "Core/Singleton.h"




using json = nlohmann::json;

class Common {
public:

    void sendToPeer(const json& message);
    void sendObjectIfChanged(const std::shared_ptr<Core::Object>& obj);
    void sendParticleIfChanged(const std::shared_ptr<Core::Particles::FireSystem>& fireSystem);

	void sendNewObject(const std::shared_ptr<Core::Object>& obj);
	void sendNewComponent(const std::shared_ptr<Core::Component>& comp);
	void sendDeleteObject(const std::shared_ptr<Core::Object>& obj);

	void sendInitScene();
	void sendMapRequest();

	void setLastSentObject(const std::shared_ptr<Core::Object>& obj);

	void sendTransferTypeUpdate(bool bulkTransfer);

	void bulkTransferScene();

	bool bulkTransfer = false;

protected:
    SOCKET serverSocket, clientSocket;
    std::shared_ptr<Core::Object> lastSentObject;
    std::shared_ptr<Core::Particles::ParticleSystem> lastSentParticleSys;

    static DWORD WINAPI ReceiveThread(LPVOID lpParam);

	//Data handling

	static void getBulkTransfer(const json& data);

    static void transformObject(const json& receivedJson);

	static void createObject(const json& data);
	static void createComponent(const json& data);
	static void deleteObject(const json& data);

    static void transformParticle(const json& data);

	static void getScene(const json& data);
	//static void transformDecal(const json& data);

};


#endif