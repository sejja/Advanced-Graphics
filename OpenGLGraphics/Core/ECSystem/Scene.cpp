//
//	Scene.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 03/03/24
//	Copyright © 2024. All Rights reserved
//
#include <iostream>
#include "Scene.h"
#include "Core/Graphics/Pipeline.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"
#include "Graphics/Primitives/Lights/PointLight.h"
#include "Graphics/Primitives/Lights/SpotLight.h"
#include "Core/Singleton.h"
#include "Graphics/Primitives/Skybox.h"
#include "Core/ParticleSystem/ParticleManager.h"
#include "Core/ParticleSystem/ParticleSystem.h"
#include "Core/ParticleSystem/FireSystem.h"
#include "Graphics/OpenGLPipeline.h"
#include "Graphics/Primitives/GLBModel.h"

#include "Core/AppWrapper.h"
#include "Core/Graphics/Pipeline.h"

#include <fstream>
#include <string>

#include "Graphics/Architecture/InstancedRendering/InstancedRendering.h"

#include "Core/Assets/ResourceManager.h"
#include "Core/Logger.h"
#include "Core/Editor/Editor.h"



namespace Core {
	// ------------------------------------------------------------------------
	/*! Create Scene
	*
	*   Creates a scene from a level file
	*/ // ---------------------------------------------------------------------
	void Scene::CreateScene(const std::string_view& file, std::function<void(const std::shared_ptr<Core::Object>& obj)> upload) {
		//mParser.LoadDataFromFile(file.data());
		auto& resmg = Singleton<Core::Assets::ResourceManager>::Instance();
		auto& instancedRenderer = Singleton<::Graphics::Architecture::InstancedRendering::InstanceRenderer>::Instance();
		auto& logger = Singleton<Logger>::Instance();
		
		//limpiar actions
		Singleton<::Editor>::Instance().GetActionManager()->Clear();

		std::ifstream f(file.data());
		json data = json::parse(f);

		//limpiar actions
		Singleton<::Editor>::Instance().GetActionManager()->Clear();

		bool hasSkybox = false;

		std::shared_ptr<Core::Particles::ParticleMangager> particleManager = std::move(std::make_shared<Core::Particles::ParticleMangager>());

		json objects = data["objects"];
		for (int i = 0; i < objects.size(); i++) {
			try {
			printf("Creating object\n");
			std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
			obj->SetName(objects[i]["name"]);
			obj->SetID(objects[i]["_id"]);
			obj->SetPosition(glm::vec3(objects[i]["position"][0], objects[i]["position"][1], objects[i]["position"][2]));
			obj->SetRotation(glm::vec3(objects[i]["rotation"][0], objects[i]["rotation"][1], objects[i]["rotation"][2]));
			obj->SetScale(glm::vec3(objects[i]["scale"][0], objects[i]["scale"][1], objects[i]["scale"][1]));

			json components = objects[i]["components"];
			//printf("Number of components: %d", components.size());
			for (int j = 0; j < components.size(); j++) {
				if (components[j]["type"] == "Model Renderer") {
					printf("\tCreating renderer\n");
					std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::make_shared<Core::Graphics::GLBModelRenderer <Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj);

					std::string mesh = components[j]["model"];
					renderer->SetMesh(resmg.GetResource<::Graphics::Primitives::Model>(mesh.c_str()));

					//if(obj->GetID() != "New Object") 
					//instancedRenderer.add_To_InstancedRendering(renderer, obj);

					std::string shader = components[j]["shader"];
					renderer->SetShaderProgram(resmg.GetResource<Graphics::ShaderProgram>(shader.c_str()));
					obj->AddComponent(std::move(renderer));
					//printf("Num componentes anadidos: %d\n", obj->GetAllComponents().size());
				}
				else if (components[j]["type"] == "Directional Light") {
					printf("\tCreating light\n");
					std::shared_ptr<::Graphics::Primitives::Lights::DirectionalLight> light = std::make_shared<::Graphics::Primitives::Lights::DirectionalLight>(obj);
					//light->mData->mColor = glm::vec3(components[j]["color"][0], components[j]["color"][1], components[j]["color"][2]);
					light->SetColor(glm::vec3(components[j]["color"][0], components[j]["color"][1], components[j]["color"][2]));
					//light->mData->mShadowCaster = components[j]["shadowCaster"];
					//((::Graphics::Primitives::DirectionalLight::DirectionalLightData*)light->mData)->mDirection = glm::vec3(components[j]["direction"][0], components[j]["direction"][1], components[j]["direction"][2]);
					light->SetDirection(glm::vec3(components[j]["direction"][0], components[j]["direction"][1], components[j]["direction"][2]));

					light->SetPosition(glm::vec3(components[j]["position"][0], components[j]["position"][1], components[j]["position"][2]), obj->GetPosition());

					obj->AddComponent(std::move(light));
				}
				else if (components[j]["type"] == "Point Light") { //Qué hago con inner, outter y fallOff?
					printf("\tCreating light\n");
					std::shared_ptr<::Graphics::Primitives::Lights::PointLight> light = std::make_shared<::Graphics::Primitives::Lights::PointLight>(obj);
					light->SetColor(glm::vec3(components[j]["color"][0], components[j]["color"][1], components[j]["color"][2]));
					//light->mData->mShadowCaster = components[j]["shadowCaster"];
					/*((::Graphics::Primitives::PointLight::PointLightData*)light->mData)->mRadius = components[j]["radius"];
					((::Graphics::Primitives::PointLight::PointLightData*)light->mData)->mInner = components[j]["innerAngle"];
					((::Graphics::Primitives::PointLight::PointLightData*)light->mData)->mOutter = components[j]["outterAngle"];*/
					light->SetRadius(components[j]["radius"]);
					light->SetFallOff(components[j]["fallOf"]);
					light->SetShadowCaster(components[j]["shadowCaster"]);

					light->SetPosition(glm::vec3(components[j]["position"][0], components[j]["position"][1], components[j]["position"][2]), obj->GetPosition());

					light->SetIsFireLight(components[j]["isFireLight"]);
					light->SetFireParams(glm::vec4(components[j]["fireParams"][0], components[j]["fireParams"][1], components[j]["fireParams"][2], components[j]["fireParams"][3]));

					obj->AddComponent(std::move(light));
				}
				else if (components[j]["type"] == "Spot Light") { //Fallof??
					std::shared_ptr<::Graphics::Primitives::Lights::SpotLight> light = std::make_shared<::Graphics::Primitives::Lights::SpotLight>(obj);
					light->SetColor(glm::vec3(components[j]["color"][0], components[j]["color"][1], components[j]["color"][2]));
					//light->mData->mShadowCaster = components[j]["shadowCaster"];
					light->SetDirection(glm::vec3(components[j]["direction"][0], components[j]["direction"][1], components[j]["direction"][2]));
					light->SetInner(components[j]["innerAngle"]);
					light->SetOuter(components[j]["outterAngle"]);
					light->SetFallOff(components[j]["fallOff"]);
					light->SetRadius(components[j]["radius"]);
					light->SetShadowCaster(components[j]["shadowCaster"]);

					light->SetPosition(glm::vec3(components[j]["position"][0], components[j]["position"][1], components[j]["position"][2]), obj->GetPosition());

					obj->AddComponent(std::move(light));
				}
				else if (components[j]["type"] == "Skybox") {
					hasSkybox = true;
					std::shared_ptr<Core::Graphics::Skybox> skybox = std::make_shared<Core::Graphics::Skybox>(obj);
					skybox->CreateCubeMap();
					skybox->LoadCubeMap(components[j]["path"]);
					obj->AddComponent(std::move(skybox));
				}
				else if (components[j]["type"] == "Decal") {
					hasSkybox = true;
					std::shared_ptr<::Graphics::Primitives::Decal> decal = std::make_shared<::Graphics::Primitives::Decal>(obj);
					decal->SetDiffuse(resmg.GetResource<Core::Graphics::Texture>(std::string(components[j]["Diffuse"]).c_str()));
					decal->SetNormal(resmg.GetResource<Core::Graphics::Texture>(std::string(components[j]["Normal"]).c_str()));
					obj->AddComponent(std::move(decal));
				}
				else if (components[j]["type"] == "Fire") {
					std::shared_ptr<Core::Particles::FireSystem> fire = std::make_shared<Core::Particles::FireSystem>(obj);
					fire->ChangeFireSize(components[j]["radiusVector"][0], components[j]["radiusVector"][1], components[j]["radiusVector"][2], components[j]["gap"], components[j]["height"]);
					fire->SetBaseColor(glm::vec4(components[j]["color"][0], components[j]["color"][1], components[j]["color"][2], components[j]["color"][3]));
					fire->SetSystemCenter(glm::vec3(components[j]["center"][0], components[j]["center"][1], components[j]["center"][2]), obj->GetPosition());
					fire->SetParticleSize(components[j]["particleSize"]);
					obj->AddComponentR(fire);
					particleManager->AddComponent(std::move(fire));
				}
				else {
					logger.logMessage(LogLevel::WARNING, "Unknown component type");
				}
			}

			upload(obj);
			mObjects.emplace_back(std::move(obj));
			}
			catch (json::exception ex) {
				std::cout << ex.what() << std::endl;
				logger.logMessage(LogLevel::WARNING, "Object could not be loaded");
			}
		}
		mObjects.emplace_back(particleManager);
		Singleton<AppWrapper>::Instance().GetPipeline().SetParticleManager(particleManager);


		if (hasSkybox == false) {
			logger.logMessage("No skybox found, creating default skybox");
			std::shared_ptr<Core::Object> sky = std::move(std::make_shared<Core::Object>());
			std::shared_ptr<Core::Graphics::Skybox> skycomp = std::make_shared<Core::Graphics::Skybox>(sky);
			skycomp->CreateCubeMap();
			sky->AddComponent(std::move(skycomp));
			sky->SetName("SKYBOX");
			mObjects.emplace_back(sky);
			//std::cout << "Creando skybox de 0" << std::endl;
		}


		instancedRenderer.fetch();

		/*Test data*/
		//std::shared_ptr<Core::Particles::ParticleMangager> particleManager = std::move(std::make_shared<Core::Particles::ParticleMangager>());
		//std::shared_ptr<Core::Particles::FireSystem> testParticleSystem = std::make_shared<Core::Particles::FireSystem>(particleManager);
		//particleManager->AddComponent(std::move(testParticleSystem));
		//mObjects.emplace_back(particleManager);
		//Singleton<AppWrapper>::Instance().GetPipeline().SetParticleManager(particleManager);
	}

	// ------------------------------------------------------------------------
	/*! Tick
	*
	*   Updates all the objects and it's components
	*/ // ---------------------------------------------------------------------
	void Scene::Tick() {
		std::for_each(std::execution::par, mObjects.begin(), mObjects.end(), [](const std::shared_ptr<Core::Object>& x) {
			x->Update();
			});
	}

	void Scene::Save(const std::string_view& filename) {
		Logger& logger = Singleton<Logger>::Instance();
		auto resmg = Singleton<Core::Assets::ResourceManager>::Instance();
		//printf("Listos para guardar\n");
		std::cout << filename << std::endl;
		json data;
		for (int i = 0; i < mObjects.size(); i++) {
			json object;
			if (mObjects[i]->GetName() == "ParticleManager") {
				continue;
			}
			object["name"] = mObjects[i]->GetName();
			object["_id"] = mObjects[i]->GetID();
			object["position"] = { mObjects[i]->GetPosition().x, mObjects[i]->GetPosition().y, mObjects[i]->GetPosition().z };
			object["rotation"] = { mObjects[i]->GetRotation().x, mObjects[i]->GetRotation().y, mObjects[i]->GetRotation().z };
			object["scale"] = { mObjects[i]->GetScale().x, mObjects[i]->GetScale().y, mObjects[i]->GetScale().z };


			std::vector<std::shared_ptr<Core::Component>> components = mObjects[i]->GetAllComponents();

			for (int j = 0; j < components.size(); j++) {
				json component;
				//data["objects"][i]["components"][j] = component;
				std::shared_ptr<Core::Component> comp = components[j];
				component["id"] = comp->GetID();
				//printf("Component type: %s\n", typeid(*comp).name());
				if (typeid(*comp) == typeid(Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>)) {
					//printf("Model\n");
					std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::static_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp);
					component["type"] = "Model Renderer";
					std::shared_ptr<std::string> str = resmg.GetResourceName<Core::Graphics::ShaderProgram>(renderer->GetShaderProgram().lock());
					//std::cout << *str << std::endl;
					component["shader"] = *str;
					component["model"] = *resmg.GetResourceName<::Graphics::Primitives::Model>(renderer->GetModel());
				}
				else if (typeid(*comp) == typeid(::Graphics::Primitives::Lights::DirectionalLight)) {
					printf("Se hizo la luz\n");
					component["type"] = "Directional Light";
					std::shared_ptr<::Graphics::Primitives::Lights::DirectionalLight> light = std::static_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(comp);
					//light->
					component["color"][0] = light->GetColor().r;
					component["color"][1] = light->GetColor().g;
					component["color"][2] = light->GetColor().b;
					//component["shadowCaster"] = light->mData->mShadowCaster;
					component["direction"][0] = light->GetDirection().x; //Comprobar que funcione
					component["direction"][1] = light->GetDirection().y;
					component["direction"][2] = light->GetDirection().z;

					component["position"][0] = light->GetPosition().x;
					component["position"][1] = light->GetPosition().y;
					component["position"][2] = light->GetPosition().z;
				}
				else if (typeid(*comp) == typeid(::Graphics::Primitives::Lights::PointLight)) {
					printf("Se hizo la luz\n");
					component["type"] = "Point Light";
					std::shared_ptr<::Graphics::Primitives::Lights::PointLight> light = std::static_pointer_cast<::Graphics::Primitives::Lights::PointLight>(comp);
					component["color"][0] = light->GetColor().r;
					component["color"][1] = light->GetColor().g;
					component["color"][2] = light->GetColor().b;
					//component["shadowCaster"] = light->mData->mShadowCaster;
					//component["shadowCaster"] = light->GetShadowCasting();
					component["radius"] = light->GetRadius();
					component["fallOf"] = light->GetFallOff();
					//component["innerAngle"] = ((::Graphics::Primitives::PointLight::PointLightData*)(light->mData))->mInner;
					//component["outterAngle"] = ((::Graphics::Primitives::PointLight::PointLightData*)(light->mData))->mOutter;
					component["shadowCaster"] = light->IsShadowCaster();

					component["position"][0] = light->GetPosition().x;
					component["position"][1] = light->GetPosition().y;
					component["position"][2] = light->GetPosition().z;

					component["isFireLight"] = light->GetIsFireLight();
					component["fireParams"][0] = light->GetFireParams().x;
					component["fireParams"][1] = light->GetFireParams().y;
					component["fireParams"][2] = light->GetFireParams().z;
					component["fireParams"][3] = light->GetFireParams().w;


				}
				else if (typeid(*comp) == typeid(::Graphics::Primitives::Lights::SpotLight)) {
					printf("Se hizo la luz\n");
					component["type"] = "Spot Light";
					std::shared_ptr<::Graphics::Primitives::Lights::SpotLight> light = std::static_pointer_cast<::Graphics::Primitives::Lights::SpotLight>(comp);
					component["color"][0] = light->GetColor().r;
					component["color"][1] = light->GetColor().g;
					component["color"][2] = light->GetColor().b;
					component["shadowCaster"] = light->GetShadowCasting();
					component["direction"][0] = light->GetDirection().x;
					component["direction"][1] = light->GetDirection().y;
					component["direction"][2] = light->GetDirection().z;
					component["innerAngle"] = light->GetInner();
					component["outterAngle"] = light->GetOutter();
					component["fallOff"] = light->GetFallOff();
					component["radius"] = light->GetRadius();

					component["position"][0] = light->GetPosition().x;
					component["position"][1] = light->GetPosition().y;
					component["position"][2] = light->GetPosition().z;
				}
				else if (typeid(*comp) == typeid(Core::Graphics::Skybox)) {
					component["type"] = "Skybox";
					component["path"] = std::static_pointer_cast<Core::Graphics::Skybox>(comp)->GetPath();
				}
				else if (typeid(*comp) == typeid(::Graphics::Primitives::Decal)) {
					component["type"] = "Decal";
					component["Diffuse"] = *resmg.GetResourceName<Core::Graphics::Texture>(std::static_pointer_cast<::Graphics::Primitives::Decal>(comp)->GetDiffuse().lock());
					component["Normal"] = *resmg.GetResourceName<Core::Graphics::Texture>(std::static_pointer_cast<::Graphics::Primitives::Decal>(comp)->GetNormal().lock());
				}
				else if (typeid(*comp) == typeid(Core::Particles::FireSystem)) {
					component["type"] = "Fire";
					std::shared_ptr<Core::Particles::FireSystem> fire = std::static_pointer_cast<Core::Particles::FireSystem>(comp);
					component["color"][0] = fire->GetBaseColor().r;
					component["color"][1] = fire->GetBaseColor().g;
					component["color"][2] = fire->GetBaseColor().b;
					component["color"][3] = fire->GetBaseColor().a;
					component["gap"] = fire->GetFireGap();
					component["height"] = fire->GetFireHeight();
					component["particleSize"] = fire->GetParticleSize();
					component["radiusVector"][0] = fire->GetRadiusVector().x;
					component["radiusVector"][1] = fire->GetRadiusVector().y;
					component["radiusVector"][2] = fire->GetRadiusVector().z;
					component["center"][0] = fire->GetSystemCenter().x;
					component["center"][1] = fire->GetSystemCenter().y;
					component["center"][2] = fire->GetSystemCenter().z;
				}
				else {
					logger.logMessage(LogLevel::WARNING, "Unknown component type");
				}
				
				
				object["components"][j] = component;
			}
			data["objects"][i] = object;
		}
		std::ofstream file;
		file.open(filename.data());

		file << data.dump(4);

		file.close();
	}

	void Scene::ClearScene()
	{
		mObjects.clear();
		Singleton<::Graphics::Architecture::InstancedRendering::InstanceRenderer>::Instance().clear();
	}


}