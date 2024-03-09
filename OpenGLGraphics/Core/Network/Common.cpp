#include "Common.h"
#include <iostream>
#include "Core/ECSystem/Scene.h"
#include "Core/AppWrapper.h"



DWORD WINAPI Common::ReceiveThread(LPVOID lpParam){
    SOCKET clientSocket = *((SOCKET*)lpParam);
    char buffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Received from peer: " << buffer << std::endl;

            // Parsear el JSON recibido
            json receivedJson;
            try {
                receivedJson = json::parse(buffer);
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl; //NO SE HA ENVIADO UN JSON VALIDO
                continue; // Saltar al siguiente msg
            }

            // Ver q tipo de msg es
            if (receivedJson.contains("type")) {
				std::string type = receivedJson["type"];
                if (type == "object_transform") {
                    // Actualizar la posición del objeto 
                    //TODO: Separar en archivo aparte
                    transformObject(receivedJson);
				}
                else if (type == "object_creation") {
                    //crear objeto
                }
                else if (type == "object_deletion") {
					//borrar objeto
				}
                else if (type == "object_property") {
                    //cambiar propiedad de objeto, luz , textura, etc
                }
                else {
					std::cerr << "Unknown type: " << type << std::endl;
				}
			}

        }
    }
}



void Common::sendToPeer(const json& message){
    std::string serialized_message = message.dump();
    send(clientSocket, serialized_message.c_str(), serialized_message.length(), 0);
}

void Common::sendObjectIfChanged(const std::shared_ptr<Core::Object>& obj){

    glm::vec3 curPos = obj->GetPosition();
    glm::vec3 curRot = obj->GetRotation();
    glm::vec3 curScale = obj->GetScale();


    bool positionChanged = true;
    bool rotationChanged = true;
    bool scaleChanged = true;


    if (lastSentObject != NULL) {
        positionChanged = curPos != lastSentObject->GetPosition();
        rotationChanged = curRot != lastSentObject->GetRotation();
        scaleChanged = curScale != lastSentObject->GetScale();
    }


    if (positionChanged || rotationChanged || scaleChanged) {

        printf("Sending object properties to client\n");
        json data = {
            {"type", "object_transform"},
            {"id", obj->GetID()},
            {"position", {curPos.x, curPos.y, curPos.z}},
            {"rotation", {curRot.x, curRot.y, curRot.z}},
            {"scale", {curScale.x, curScale.y, curScale.z}}
        };

        sendToPeer(data);

        lastSentObject = std::make_shared<Core::Object>(*obj);
    }
}



std::shared_ptr<Core::Object> getObjectByID(const std::string& objectID) {
    Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
    for (const auto& obj : scene.GetObjects()) {
        if (obj->GetID() == objectID) {
			return obj;
		}
	}
	return NULL;

}


void Common::transformObject(const json& data) {
    std::shared_ptr<Core::Object> obj = getObjectByID(data["id"]);
    if (obj != NULL) {
		glm::vec3 newPos = { data["position"][0], data["position"][1], data["position"][2] };
		glm::vec3 newRot = { data["rotation"][0], data["rotation"][1], data["rotation"][2] };
		glm::vec3 newScale = { data["scale"][0], data["scale"][1], data["scale"][2] };

		obj->SetPosition(newPos);
		obj->SetRotation(newRot);
		obj->SetScale(newScale);
	}
    else {
		std::cerr << "Object not found" << std::endl;
	}
}