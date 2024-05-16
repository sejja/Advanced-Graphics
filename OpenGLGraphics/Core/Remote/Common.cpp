#include "Common.h"
#include <iostream>
#include "Core/ECSystem/Scene.h"
#include "Core/AppWrapper.h"
#include "NetManager.h"
#include "Core/Editor/Editor.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"
#include "Graphics/Primitives/Lights/PointLight.h"
#include "Graphics/Primitives/Lights/SpotLight.h"
#include <fstream>


enum class MessageType {
    ObjectTransform,
    ObjectCreation,
    ObjectDeletion,
    ObjectProperty,
	ComponentCreation,
    ParticleTransform,
    TransferTypeUpdate,
    InitScene,
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
    else if (typeStr == "component_creation") {
        return MessageType::ComponentCreation;
    }
    else if (typeStr == "particle_transform") {
        return MessageType::ParticleTransform;
    }
	else if (typeStr == "transfer_type_update") {
		return MessageType::TransferTypeUpdate;
	}
    else if (typeStr == "init_scene") {
        return MessageType::InitScene;
    }
    else {
        return MessageType::Unknown;
    }
}



DWORD WINAPI Common::ReceiveThread(LPVOID lpParam) {
    SOCKET clientSocket = *((SOCKET*)lpParam);
    char buffer[4096];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(clientSocket, buffer, 4096, 0);
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
                case MessageType::InitScene:
					getScene(receivedJson);
					break;
                case MessageType::ObjectTransform:
                    // Actualizar la posici?n del objeto
                    transformObject(receivedJson);
                    break;
                case MessageType::ObjectCreation:
                    createObject(receivedJson);
                    break;
                case MessageType::ObjectDeletion:
                    // Borrar objeto
                    deleteObject(receivedJson);
                    break;
				case MessageType::ComponentCreation:
					createComponent(receivedJson);
					break;
                case MessageType::ObjectProperty:
                    // Cambiar propiedad de objeto, luz , textura, etc
                    //changeObjectProperty(receivedJson);
                    break;
                case MessageType::ParticleTransform:
                    // Actualizar la posici?n de una particula
                    transformParticle(receivedJson);
                    break;
                case MessageType::Unknown:
                    std::cerr << "Tipo de mensaje rarete: :???? -> " << receivedJson["type"] << std::endl;
                    break;
                }
            }


        }
    }
}




void Common::sendToPeer(const json& message) {
    std::string serialized_message = message.dump();
    send(clientSocket, serialized_message.c_str(), serialized_message.length(), 0);
}


void Common::sendParticleIfChanged(const std::shared_ptr<Core::Particles::FireSystem>& fireSystem) {
    glm::vec3 curCenter = fireSystem->GetSystemCenter();
    glm::vec4 curColor = fireSystem->GetBaseColor();
    glm::vec3 curRadius = fireSystem->GetRadiusVector();
    float curGap = fireSystem->GetFireGap();
    float curHeight = fireSystem->getHeigth();
    float curParticleSize = fireSystem->GetParticleSize();


    bool centerChanged = true, colorChanged = true, radiusChanged = true, gapChanged = true, heightChanged = true, particleSizeChanged = true;

    std::shared_ptr<Core::Particles::FireSystem> lastSentFireSys = std::dynamic_pointer_cast<Core::Particles::FireSystem>(lastSentParticleSys);


    if (lastSentFireSys != NULL) {
        printf("checking change");
        centerChanged = curCenter != lastSentFireSys->GetSystemCenter();
        colorChanged = curColor != lastSentFireSys->GetBaseColor();
        radiusChanged = curRadius != lastSentFireSys->GetRadiusVector();
        gapChanged = curGap != lastSentFireSys->GetFireGap();
        heightChanged = curHeight != lastSentFireSys->getHeigth();
    }

    if (centerChanged || colorChanged || radiusChanged || gapChanged || heightChanged) {
        //printf("Sending particle system properties to client\n");

        printf("centerChanged | colorChanged | radiusChanged | gapChanged | heightChanged\n %d %d %d %d %d\n", centerChanged, colorChanged, radiusChanged, gapChanged, heightChanged);
        json data = {
            {"type", "particle_transform"},
            {"id", fireSystem->GetID()},
            {"center", {curCenter.x, curCenter.y, curCenter.z}},
            {"color", {curColor.r, curColor.g, curColor.b, curColor.a}},
            {"radius", {curRadius.x, curRadius.y, curRadius.z}},
            {"gap", curGap},
            {"height", curHeight},
            {"particleSize", curParticleSize}

        };

        sendToPeer(data);

        lastSentParticleSys = std::make_shared<Core::Particles::ParticleSystem>(*fireSystem);
    }
}

void Common::sendNewObject(const std::shared_ptr<Core::Object>& obj)
{
	glm::vec3 curPos = obj->GetPosition();
	glm::vec3 curRot = obj->GetRotation();
	glm::vec3 curScale = obj->GetScale();

	json data = {
		{"type", "object_creation"},
		{"id", obj->GetID()},
        {"name", obj->GetName()},
		{"position", {curPos.x, curPos.y, curPos.z}},
		{"rotation", {curRot.x, curRot.y, curRot.z}},
		{"scale", {curScale.x, curScale.y, curScale.z}}
	};

	sendToPeer(data);
}

void Common::sendNewComponent(const std::shared_ptr<Core::Component>& comp){
    auto lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::Light>(comp);
    auto meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp);
    auto fireSystem = std::dynamic_pointer_cast<Core::Particles::FireSystem>(comp);
    auto decal = std::dynamic_pointer_cast<Graphics::Primitives::Decal>(comp);
	std::string ctype = "";
    if (lightComp) {ctype = "Light";}
    else if (meshComp) {ctype = "Mesh";}
    else if (fireSystem) {ctype = "FireSystem";}
    else if (decal) {ctype = "Decal";}
    else {
		std::cerr << "Component type not recognized" << std::endl;
        return;
	}

	json data = {
		{"type", "component_creation"},
		{"CompType", ctype},
		{"compID", comp->GetID()},
        {"lightType", lightComp ? "Point" : "NONE"},
		{"objID", comp->GetParent().lock()->GetID()}
	};

	sendToPeer(data);
}

void Common::sendDeleteObject(const std::shared_ptr<Core::Object>& obj){
	json data = {
		{"type", "object_deletion"},
		{"id", obj->GetID()}
	};
	sendToPeer(data);
}

void Common::sendInitScene(){
    Singleton<AppWrapper>::Instance().getScene().Save("Core/Remote/BulkTransfer/scene.json");

    std::ifstream file("Core/Remote/BulkTransfer/scene.json");
    if (!file.is_open()) {
        std::cerr << "Cant open save /bulktransfer/scene.json" << std::endl;
        return;
    }
    json sceneData;
    file >> sceneData;
    file.close();

    json data = {
        {"type", "init_scene"},
        {"scene", sceneData}
    };

    std::cout << "DATA" << data << std::endl;
    sendToPeer(data);
}

void Common::sendMapRequest(){

}

void Common::sendObjectIfChanged(const std::shared_ptr<Core::Object>& obj) {
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
			{"name", obj->GetName()},
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

std::shared_ptr<Core::Component> getComponentByID(std::shared_ptr<Core::Object> obj, const std::string& compID) {
    std::vector<std::shared_ptr<Core::Component>> comps = obj->GetAllComponents();
    for (auto& comp : comps) {
        if (comp->GetID() == compID) {
            return comp;
        }
    }
    return NULL;

}


std::shared_ptr<Core::Particles::ParticleSystem> getParticleSysByID(const std::string& objectID) {
    Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
    for (const auto& obj : scene.GetObjects()) {
        if (obj->GetID() == "PARTICLE_MANAGER") {
            for (const auto& comp : obj->GetAllComponents()) {
                std::shared_ptr<Core::Particles::ParticleSystem> particleSys = std::dynamic_pointer_cast<Core::Particles::ParticleSystem>(comp);
                if (particleSys->GetID() == objectID) {
                    return particleSys;
                }
            }
        }
    }
    return NULL;
}

void Common::setLastSentObject(const std::shared_ptr<Core::Object>& obj) {
	lastSentObject = std::make_shared<Core::Object>(*obj);
}

void Common::sendTransferTypeUpdate(bool bulkTransfer){
	json data = {
		{"type", "transfer_type_update"},
		{"bulkTransfer", bulkTransfer}
	};
	sendToPeer(data);

	if (bulkTransfer) {
		bulkTransferScene();
	}
}

void Common::bulkTransferScene() {
    Singleton<AppWrapper>::Instance().getScene().Save("Core/Remote/BulkTransfer/scene.json");

    std::ifstream file("Core/Remote/BulkTransfer/scene.json");
    if (!file.is_open()) {
        std::cerr << "Cant open save /bulktransfer/scene.json" << std::endl;
        return;
    }
    json sceneData;
    file >> sceneData;
    file.close();

    json data = {
        {"type", "bulk_transfer"},
        {"scene", sceneData}
    };

	std::cout << "DATA" << data << std::endl;
    sendToPeer(data);
}

void Common::getBulkTransfer(const json& data){
    auto& resmg = Singleton<Core::Assets::ResourceManager>::Instance();
    json sceneData = data["scene"];
    json objects = sceneData["objects"];

    for (int i = 0; i < objects.size(); i++) {
        try {
            printf("Updating object\n");

            std::shared_ptr<Core::Object> obj = getObjectByID(objects[i]["_id"]);
            obj->SetName(objects[i]["name"]);
            obj->SetPosition(glm::vec3(objects[i]["position"][0], objects[i]["position"][1], objects[i]["position"][2]));
            obj->SetRotation(glm::vec3(objects[i]["rotation"][0], objects[i]["rotation"][1], objects[i]["rotation"][2]));
            obj->SetScale(glm::vec3(objects[i]["scale"][0], objects[i]["scale"][1], objects[i]["scale"][1]));

            json components = objects[i]["components"];

            for (int j = 0; j < components.size(); j++) {

                auto comp = getComponentByID(obj, components[j]["id"]);


                auto lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::Light>(comp);
                auto fireSystem = std::dynamic_pointer_cast<Core::Particles::FireSystem>(comp);
                auto decal = std::dynamic_pointer_cast<Graphics::Primitives::Decal>(comp);

                if (components[j]["type"] == "Model Renderer") {
                    auto meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp);
                    std::string mesh = components[j]["model"];
                    std::string shader = components[j]["shader"];
                    meshComp->SetMesh(resmg.GetResource<::Graphics::Primitives::Model>(mesh.c_str()));
                    meshComp->SetShaderProgram(resmg.GetResource<Core::Graphics::ShaderProgram>(shader.c_str()));
                }
                else if (components[j]["type"] == "Directional Light") {
                    printf("\tCreating light\n");
                    std::shared_ptr<::Graphics::Primitives::Lights::DirectionalLight> light = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(comp);
                    light->SetColor(glm::vec3(components[j]["color"][0], components[j]["color"][1], components[j]["color"][2]));
                    light->SetDirection(glm::vec3(components[j]["direction"][0], components[j]["direction"][1], components[j]["direction"][2]));
                }
                else if (components[j]["type"] == "Point Light") {
                    printf("\tCreating light\n");
                    std::shared_ptr<::Graphics::Primitives::Lights::PointLight> light = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::PointLight>(comp);
                    light->SetColor(glm::vec3(components[j]["color"][0], components[j]["color"][1], components[j]["color"][2]));
                    light->SetRadius(components[j]["radius"]);
                    light->SetFallOff(components[j]["fallOf"]);
                }
                else if (components[j]["type"] == "Spot Light") {
                    std::shared_ptr<::Graphics::Primitives::Lights::SpotLight> light = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::SpotLight>(comp);
                    light->SetColor(glm::vec3(components[j]["color"][0], components[j]["color"][1], components[j]["color"][2]));
                    light->SetDirection(glm::vec3(components[j]["direction"][0], components[j]["direction"][1], components[j]["direction"][2]));
                    light->SetInner(components[j]["innerAngle"]);
                    light->SetOuter(components[j]["outterAngle"]);
                    light->SetFallOff(components[j]["fallOff"]);
                    light->SetRadius(components[j]["radius"]);
                    light->SetShadowCaster(components[j]["shadowCaster"]);
                }
                else if (components[j]["type"] == "Decal") {
                    std::shared_ptr<::Graphics::Primitives::Decal> decal = std::dynamic_pointer_cast<::Graphics::Primitives::Decal>(comp);
                    decal->SetDiffuse(resmg.GetResource<Core::Graphics::Texture>(std::string(components[j]["Diffuse"]).c_str()));
                    decal->SetNormal(resmg.GetResource<Core::Graphics::Texture>(std::string(components[j]["Normal"]).c_str()));
                }
            }

        }
        catch (std::exception ex) {
            std::cout << ex.what() << std::endl;
        }
    }
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

        auto net = Singleton<NetManager>::Instance();
        if (net.isServer()) {net.GetServer().setLastSentObject(obj);}
        else {net.GetClient().setLastSentObject(obj);}

    }
    else {
        std::cerr << "Object not found" << std::endl;
    }
}

void Common::createObject(const json& data)
{
	std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
	obj->SetID(data["id"]);
	obj->SetName(data["name"]);
	obj->SetPosition({ data["position"][0], data["position"][1], data["position"][2] });
	obj->SetRotation({ data["rotation"][0], data["rotation"][1], data["rotation"][2] });
	obj->SetScale({ data["scale"][0], data["scale"][1], data["scale"][2] });

	Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
	scene.addObject(obj);

}

void Common::createComponent(const json& data)
{
	std::shared_ptr<Core::Object> obj = getObjectByID(data["objID"]);
	if (obj != NULL) {
		std::shared_ptr<Core::Component> comp;

		if (data["CompType"] == "Light") {
            std::shared_ptr<::Graphics::Primitives::Lights::PointLight> light;
            light = std::move(std::make_shared<::Graphics::Primitives::Lights::PointLight>(obj));
            light->SetRadius(1.0f);
            light->SetFallOff(0.5f);
            light->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
            glm::vec3 relativePos = glm::vec3(0.0f, 0.0f, 0.0f);
            light->SetPosition(relativePos, obj->GetPosition());
            obj->AddComponent(std::move(light));
			light->SetID(data["compID"]);
		}
		else if (data["CompType"] == "Mesh") {
            auto renderer = std::make_shared<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj);
            obj->AddComponent(std::move(renderer));
			renderer->SetID(data["compID"]);
		}
		else if (data["CompType"] == "FireSystem") {
            auto& pipeline = Singleton<AppWrapper>::Instance().GetPipeline();
            auto particleManager = pipeline.GetParticleManager();
            std::shared_ptr<Core::Particles::FireSystem> fireSystem = std::make_shared<Core::Particles::FireSystem>(obj);
            fireSystem->SetSystemCenter(glm::vec3(0.0f, 0.0f, 0.0f), obj->GetPosition());
            obj->AddComponentR(fireSystem);
            particleManager->AddComponent(std::move(fireSystem));
            fireSystem->SetID(data["compID"]);
		}
		else if (data["CompType"] == "Decal") {
            auto decal = std::make_shared<Graphics::Primitives::Decal>(obj);
            obj->AddComponent(std::move(decal));
			decal->SetID(data["compID"]);
		}
		else {
			std::cerr << "Component type not recognized" << std::endl;
			return;
		}
	}
	else {
		std::cerr << "Object not found" << std::endl;
	}
}

void Common::deleteObject(const json& data){
	std::shared_ptr<Core::Object> obj = getObjectByID(data["id"]);
	if (obj != NULL) {
		Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
		scene.removeObject(obj);

        if (Singleton<Editor>::Instance().GetSelectedObj().GetSelectedObject()->GetID()==obj->GetID()) {
			Singleton<Editor>::Instance().GetSelectedObj().SetSelectedObject(NULL);
            Singleton<Editor>::Instance().GetSelectedObj().SetSelectedComponent(NULL);
            Singleton<Editor>::Instance().GetSelectedObj().SetSelectedMesh(NULL);
        }
	}
	else {
		std::cerr << "Object not found" << std::endl;
	}
}


void Common::transformParticle(const json& data) {
    std::shared_ptr<Core::Particles::ParticleSystem> particleSys = getParticleSysByID(data["id"]);
    //particlesys a firesys
    std::shared_ptr<Core::Particles::FireSystem> fireSys = std::dynamic_pointer_cast<Core::Particles::FireSystem>(particleSys);

    if (fireSys) {
        glm::vec3 newCenter = { data["center"][0], data["center"][1], data["center"][2] };
        glm::vec4 newColor = { data["color"][0], data["color"][1], data["color"][2], data["color"][3] };
        glm::vec3 newRadius = { data["radius"][0], data["radius"][1], data["radius"][2] };
        float newGap = data["gap"];
        float newHeight = data["height"];
        float newParticleSize = data["particleSize"];

        //fireSys->SetSystemCenter(newCenter);
        fireSys->SetBaseColor(newColor);
        fireSys->ChangeFireSize(newRadius[0], newRadius[1], newRadius[2], newGap, newHeight);
        fireSys->SetParticleSize(newParticleSize);
    }
    else {
        std::cerr << "Particle system not found" << std::endl;
    }
}

void Common::getScene(const json& data){
    auto& app = Singleton<AppWrapper>::Instance();
    app.GetPipeline().ClearPipeline();
    app.getScene().ClearScene();


	json scene = data["scene"];

	std::cout << "SCEENENNENENENE : " << scene << std::endl;

    std::ofstream file("Core/Remote/BulkTransfer/scene.json");
    if (file.is_open()) {
        file << scene.dump(4);
        file.close();
    }
    else {
        std::cerr << "No se pudo abrir el archivo para escritura" << std::endl;
    }

    app.getScene().CreateScene("Content/Maps/GreatSponza.real", [&app](const std::shared_ptr<Core::Object>& obj) {
        obj->ForEachComponent([&app](const std::shared_ptr<Core::Component>& comp) {
            std::shared_ptr<Core::Graphics::Renderable> renderable = std::dynamic_pointer_cast<Core::Graphics::Renderable>(comp);
            //If the object is a renderable
            if (renderable) app.GetPipeline().AddRenderable(renderable);
            });
        });  
}
