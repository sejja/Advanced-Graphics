#include "Common.h"
#include <iostream>
#include "Core/ECSystem/Scene.h"
#include "Core/AppWrapper.h"

enum class MessageType {
    ObjectTransform,
    ObjectCreation,
    ObjectDeletion,
    ObjectProperty,
    Unknown
};


//Convierte el string del tipo de mensaje a un enum
MessageType getMessageType(const std::string& typeStr) {
    if (typeStr == "object_transform") {
        return MessageType::ObjectTransform;
    }
    else if (typeStr == "object_creation") {
        return MessageType::ObjectCreation;
    }
    else if (typeStr == "object_deletion") {
        return MessageType::ObjectDeletion;
    }
    else if (typeStr == "object_property") {
        return MessageType::ObjectProperty;
    }
    else {
        return MessageType::Unknown;
    }
}



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

                MessageType messageType = getMessageType(receivedJson["type"]);

                switch (messageType) {
                case MessageType::ObjectTransform:
                    // Actualizar la posición del objeto
                    transformObject(receivedJson);
                    break;
                case MessageType::ObjectCreation:
                    // Crear objeto
                    //createObject(receivedJson);
                    break;
                case MessageType::ObjectDeletion:
                    // Borrar objeto
                    //deleteObject(receivedJson);
                    break;
                case MessageType::ObjectProperty:
                    // Cambiar propiedad de objeto, luz , textura, etc
                    //changeObjectProperty(receivedJson);
                    break;
                case MessageType::Unknown:
                    std::cerr << "Tipo de mensaje rarete: :???? -> " << receivedJson["type"] << std::endl;
                    break;
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


    bool positionChanged = true, rotationChanged = true, scaleChanged = true;



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