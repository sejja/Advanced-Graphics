//
//	Scene.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 03/03/24
//	Copyright © 2024. All Rights reserved
//

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
#include "Dependencies/Json/single_include/json.hpp"
#include <fstream>
#include <string>

#include "Core/ResourceManager.h"

using json = nlohmann::json;

namespace Core {
	// ------------------------------------------------------------------------
	/*! Create Scene
	*
	*   Creates a scene from a level file
	*/ // ---------------------------------------------------------------------
	void Scene::CreateScene(const std::string_view& file, std::function<void(const std::shared_ptr<Core::Object>& obj)> upload) {
		mParser.LoadDataFromFile(file.data());
		auto& resmg = Singleton<ResourceManager>::Instance();

		//std::for_each(std::execution::unseq, mParser.objects.begin(), mParser.objects.end(), [this, &upload, &resmg](const SceneParser::Transform& x) {
		//	std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
		//	obj->SetPosition(x.pos);
		//	obj->SetRotation(glm::radians(x.rot));
		//	obj->SetScale(x.sca);
		//	obj->SetName(x.name);
		//	obj->SetID(x.name);//temp , tiene que ser unico
		//	//obj->SetType() tiene que ser un enum

		//	std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::make_shared<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj);
		//	renderer->SetMesh(resmg.GetResource<::Graphics::Primitives::GLBModel>(x.mesh.c_str()));

		//	if (x.name == "suzanne_mesh")
		//		renderer->SetShaderProgram(resmg.GetResource<Graphics::ShaderProgram>("Content/Shaders/Refractive.shader"));
		//	else
		//		renderer->SetShaderProgram(resmg.GetResource<Graphics::ShaderProgram>("Content/Shaders/DeferredGeometry.shader"));

		//	obj->AddComponent(std::move(renderer));
		//	upload(obj);
		//	mObjects.emplace_back(std::move(obj));
		//	});


		/*std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();*/

		std::ifstream f("Content/Maps/Scene.json");
		json data = json::parse(f);

		json objects = data["objects"];
		for (int i = 0; i < objects.size(); i++) {
			printf("Creating object\n");
			std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
			obj->SetName(objects[i]["name"]);
			obj->SetID(objects[i]["_id"]);
			obj->SetPosition(glm::vec3(objects[i]["position"][0], objects[i]["position"][1], objects[i]["position"][2]));
			obj->SetRotation(glm::vec3(objects[i]["rotation"][0], objects[i]["rotation"][1], objects[i]["rotation"][1]));
			obj->SetScale(glm::vec3(objects[i]["scale"][0], objects[i]["scale"][1], objects[i]["scale"][1]));

			json components = objects[i]["components"];
			//printf("Number of components: %d", components.size());
			for (int j = 0; j < components.size(); j++) { //Por algún motivo no se añade el segundo renderer
				if (components[j]["type"] == "renderer") {
					printf("\tCreating renderer\n");
					std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::make_shared<Core::Graphics::GLBModelRenderer <Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj);

					std::string mesh = components[j]["mesh"];
					renderer->SetMesh(resmg.GetResource<::Graphics::Primitives::GLBModel>(mesh.c_str()));

					std::string shader = components[j]["shader"];
					renderer->SetShaderProgram(resmg.GetResource<Graphics::ShaderProgram>(shader.c_str()));
					obj->AddComponent(std::move(renderer));
					//printf("Num componentes anadidos: %d\n", obj->GetAllComponents().size());
				}
			}

			upload(obj);
			mObjects.emplace_back(std::move(obj));

		}

		/*std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::make_shared< Core::Graphics::GLBModelRenderer <Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj);

		renderer->SetMesh(resmg.GetResource<::Graphics::Primitives::GLBModel>("Content/Meshes/cube_averaged.obj"));
		renderer->SetShaderProgram(resmg.GetResource<Graphics::ShaderProgram>("Content/Shaders/DeferredGeometry.shader"));

		obj->AddComponent(std::move(renderer));
		upload(obj);
		mObjects.emplace_back(std::move(obj));*/


		int i = 0;

		std::for_each(std::execution::seq, mParser.lights.begin(), mParser.lights.end(), [this, &i, &upload](const SceneParser::Light& x) {
			std::shared_ptr<Core::Object> obj = std::move(std::make_shared<Core::Object>());
			obj->SetPosition(x.pos);
			obj->SetRotation(glm::vec3(0.f, 0.f, 0.f));
			obj->SetScale({ 1.f, 1.f, 1.f });
			std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::move(std::make_shared<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj));
			std::shared_ptr<::Graphics::Primitives::Light> light;
			renderer->SetMesh(Singleton<ResourceManager>::Instance().GetResource<::Graphics::Primitives::GLBModel>("Content/Meshes/sphere_20_averaged.obj"));
			renderer->SetShaderProgram(Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/CascadedShadowMap.shader"));

			//TEMPORAL PARA SABER SI ES LUZ HASTA NUEVO LEVEL 
			obj->SetName(x.type + " Light_light");
			
			//If the light is a point light
			if (x.type == "POINT") {
				light = std::move(std::make_shared<::Graphics::Primitives::PointLight>(obj));
				((::Graphics::Primitives::PointLight::PointLightData*)light->mData)->mRadius = x.att.x;
				((::Graphics::Primitives::PointLight::PointLightData*)light->mData)->mInner = x.inner;
				((::Graphics::Primitives::PointLight::PointLightData*)light->mData)->mOutter = x.outer;
				((::Graphics::Primitives::PointLight::PointLightData*)light->mData)->mFallOff = x.falloff;
			}

			//If the light is a directional light
			else if (x.type == "DIR") {
				light = std::move(std::make_shared<::Graphics::Primitives::DirectionalLight>(obj));
				((::Graphics::Primitives::DirectionalLight::DirectionalLightData*)light->mData)->mDirection = glm::normalize(x.dir);
			}

			//else, it's a spot light
			else {
				light = std::move(std::make_shared<::Graphics::Primitives::SpotLight>(obj));
				((::Graphics::Primitives::SpotLight::SpotLightData*)light->mData)->mShadowCaster = 1;
				((::Graphics::Primitives::SpotLight::SpotLightData*)light->mData)->mRadius = x.att.x;
				((::Graphics::Primitives::SpotLight::SpotLightData*)light->mData)->mInner = x.inner;
				((::Graphics::Primitives::SpotLight::SpotLightData*)light->mData)->mOutter = x.outer;
				((::Graphics::Primitives::SpotLight::SpotLightData*)light->mData)->mFallOff = x.falloff;
				((::Graphics::Primitives::SpotLight::SpotLightData*)light->mData)->mDirection = x.dir;
			}

			light->SetPosition(x.pos);
			light->mData->mColor = x.col;

			std::weak_ptr< Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> weakrend = renderer;
			std::weak_ptr< ::Graphics::Primitives::Light> lightrend = light;

			obj->AddComponent(std::move(weakrend));
			obj->AddComponent(std::move(lightrend));

			i++;
			upload(obj);
			mObjects.emplace_back(std::move(obj));
			

			});

		std::shared_ptr<Core::Object> sky = std::move(std::make_shared<Core::Object>());
		std::shared_ptr<Core::Graphics::Skybox> skycomp = std::make_shared<Core::Graphics::Skybox>(sky);
		skycomp->CreateCubeMap();
		sky->AddComponent(std::move(skycomp));
		sky->SetName("Sky_bg");
		mObjects.emplace_back(sky);

		/*Test data*/
		std::shared_ptr<Core::Particles::ParticleMangager> particleManager = std::move(std::make_shared<Core::Particles::ParticleMangager>());
		std::shared_ptr<Core::Particles::FireSystem> testParticleSystem = std::make_shared<Core::Particles::FireSystem>(particleManager);
		particleManager->AddComponent(std::move(testParticleSystem));
		mObjects.emplace_back(particleManager);
		Singleton<AppWrapper>::Instance().GetPipeline().SetParticleManager(particleManager);
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

	void Scene::Save() {
		auto resmg = Singleton<ResourceManager>::Instance();
		printf("Listos para guardar\n");
		json data;
		for (int i = 0; i < mObjects.size(); i++) {
			json object;
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
				if (typeid(*comp) == typeid(Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>)) {
					printf("Model\n");
					std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::static_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp);
					component["type"] = "Model Renderer";
					std::shared_ptr<std::string> str = resmg.GetResourceName<Core::Graphics::ShaderProgram>(renderer->GetShaderProgram().lock());
					std::cout << *str << std::endl;
					component["shader"] = *str;
					component["model"] = *resmg.GetResourceName<::Graphics::Primitives::GLBModel>(renderer->GetMesh().lock());

					
				}

				if (typeid(*comp) == typeid(C))
				
				object["components"][j] = component;
			}
			data["objects"][i] = object;
		}
		std::ofstream file;
		file.open("Content/Maps/Scene2.json");

		file << data;

		file.close();
	}

}