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

#include "Core/Assets/ResourceManager.h"
#include "Core/Editor/Editor.h"
#include "Core/Editor/SelectedObj.h"
#include <gtx/euler_angles.hpp>
#include "Graphics/Architecture/SpacePartitioning/geometry.hpp"


using json = nlohmann::json;

namespace Core {
	// ------------------------------------------------------------------------
	/*! Create Scene
	*
	*   Creates a scene from a level file
	*/ // ---------------------------------------------------------------------
	void Scene::CreateScene(const std::string_view& file, std::function<void(const std::shared_ptr<Core::Object>& obj)> upload) {
		//mParser.LoadDataFromFile(file.data());
		auto& resmg = Singleton<Core::Assets::ResourceManager>::Instance();
		
		bool hasSkybox = false;

		std::ifstream f(file.data());
		json data = json::parse(f);

		//limpiar actions
		Singleton<::Editor>::Instance().GetActionManager()->Clear();

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
				}

				upload(obj);
				mObjects.emplace_back(std::move(obj));
			}
			catch (std::exception ex) {
				std::cout << ex.what() << std::endl;
			}
		}


		if (hasSkybox == false) {
			std::shared_ptr<Core::Object> sky = std::move(std::make_shared<Core::Object>());
			std::shared_ptr<Core::Graphics::Skybox> skycomp = std::make_shared<Core::Graphics::Skybox>(sky);
			skycomp->CreateCubeMap();
			sky->AddComponent(std::move(skycomp));
			sky->SetName("SKYBOX");
			mObjects.emplace_back(sky);
			std::cout << "Creando skybox de 0" << std::endl;
		}


		/*Test data*/
		std::shared_ptr<Core::Particles::ParticleMangager> particleManager = std::move(std::make_shared<Core::Particles::ParticleMangager>());
		//std::shared_ptr<Core::Particles::FireSystem> testParticleSystem = std::make_shared<Core::Particles::FireSystem>(particleManager);
		//particleManager->AddComponent(std::move(testParticleSystem));
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

	void Scene::Save(const std::string_view& filename) {
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
				}
				else if (typeid(*comp) == typeid(Core::Graphics::Skybox)) {
					component["type"] = "Skybox";
					component["path"] = std::static_pointer_cast<Core::Graphics::Skybox>(comp)->GetPath();
				}
				else if (typeid(*comp) == typeid(::Graphics::Primitives::Decal)) {
					component["type"] = "Decal";
					component["Diffuse"] = *resmg.GetResourceName<Core::Graphics::Texture>(std::static_pointer_cast<::Graphics::Primitives::Decal>(comp)->GetDiffuse().lock());
					component["Normal"] = *resmg.GetResourceName<Core::Graphics::Texture>(std::static_pointer_cast<::Graphics::Primitives::Decal>(comp)->GetNormal().lock());
				} else if (typeid(*comp) == typeid(Core::Particles::FireSystem)) {
					component["type"] = "Fire";
					std::shared_ptr<Core::Particles::FireSystem> fire = std::static_pointer_cast<Core::Particles::FireSystem>(comp);
					component["color"][0] = fire->GetBaseColor().r;
					component["color"][1] = fire->GetBaseColor().g;
					component["color"][2] = fire->GetBaseColor().b;
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
	}











	void Scene::AddObjToNode(::Graphics::Primitives::Mesh& mesh, Octree<::Graphics::Primitives::Mesh>::node* node) {
		if (mesh.m_octree_node->first == nullptr) {
			mesh.m_octree_node->first = &mesh; //set this object as the first object
		}
		else {
			bool flag = false; //flag to check if the object was already inserted

			::Graphics::Primitives::Mesh* pointer = mesh.m_octree_node->first;

			while (pointer->m_octree_next_obj != nullptr) {
				pointer = pointer->m_octree_next_obj;
				if (pointer->getMeshID() == mesh.getMeshID()) {//same object
					flag = true;
					break;
				}
			}
			//set the pointers if the object was not already inside, check also with the first and last objects
			if (!flag && mesh.m_octree_node->first->getMeshID() != mesh.getMeshID()) {
				pointer->m_octree_next_obj = &mesh;
				mesh.m_octree_prev_obj = pointer;
			}
		}


	}

	void Scene::EraseObjFromPrevNode(::Graphics::Primitives::Mesh& mesh, Octree<::Graphics::Primitives::Mesh>& tree) {
		//traverse through the linked list, in order to erase it from the prev linked list
		::Graphics::Primitives::Mesh* pointer = mesh.m_octree_node->first;
		if (mesh.m_octree_node->first) {
			//if it was in the 1 position
			if (mesh.m_octree_node->first->getMeshID() == mesh.getMeshID()) {
				if (pointer->m_octree_next_obj == nullptr) //alone in the linked list
				{
					mesh.m_octree_node->first = nullptr;
					if (mesh.m_octree_node->children_active == 0) //if not children nodes delete
						tree.delete_node(mesh.m_octree_node->locational_code);

				}
				else { //stil more objects in the linked list
					mesh.m_octree_node->first = mesh.m_octree_next_obj;
					mesh.m_octree_next_obj = nullptr;
				}
			}
			else {
				while (pointer->m_octree_next_obj != nullptr) {
					//traverse
					pointer = pointer->m_octree_next_obj;

					if (pointer->getMeshID() == mesh.getMeshID()) {
						//if it is in the last position
						if (pointer->m_octree_next_obj == nullptr) {
							pointer->m_octree_prev_obj->m_octree_next_obj = nullptr;
						}
						else {
							pointer->m_octree_prev_obj->m_octree_next_obj = pointer->m_octree_next_obj;
							pointer->m_octree_next_obj->m_octree_prev_obj = pointer->m_octree_prev_obj;
						}

						//set to null the pointers of the object
						mesh.m_octree_prev_obj = nullptr;
						mesh.m_octree_next_obj = nullptr;
					}
				}
			}
		}
	}
	
	void Scene::CheckFrustrumObjectCollisions(Octree<::Graphics::Primitives::Mesh>::node* node, frustrum const& frus){
		::Graphics::Primitives::Mesh* pointer = node->first;

		pointer->visible = true;
	}
	

	void Scene::OctreeCheck(frustrum const& frustum){
		stat_frustum_aabb_checks = 0;
		stat_frustum_aabb_positive = 0;

		for (auto& it : m_octree.m_nodes) {
			if (it.second->first) {//if it has objects inside
				aabb node = LocationalCode::compute_bv(it.second->locational_code, m_octree.root_size());
				eResult c = ::classify_frustum_aabb_naive(frustum, node);
				stat_frustum_aabb_checks++;

				if (c == eINSIDE) {
					::Graphics::Primitives::Mesh* pointer = it.second->first;
					while (pointer) {
						pointer->visible = true;
						pointer = pointer->m_octree_next_obj;
						stat_frustum_aabb_positive++;
					}
				}
				else if (c == eOUTSIDE) {
					::Graphics::Primitives::Mesh* pointer = it.second->first;
					while (pointer) {
						pointer->visible = false;
						pointer = pointer->m_octree_next_obj;
					}
				}
				else {// overlaping
					CheckFrustrumObjectCollisions(it.second, frustum);
					std::cout << "INICIANDO COLISIONES" << std::endl;
				}
			}
		}
	}

	void Scene::FrustumCheck(frustrum const& frustum)
	{
		// Stats
		stat_frustum_aabb_checks = 0;
		stat_frustum_aabb_positive = 0;

		// [TODO]
		for (auto& x : m_octree.m_nodes) {
			CheckFrustrumObjectCollisions(x.second, frustum);
			std::cout << "INICIANDO COLISIONES en FRUSTUMCHECK" << std::endl;
		}
	}


	aabb scaleAndPositionAABB(const aiAABB& originalAABB, const glm::vec3& scale, const glm::vec3& position) {
		aabb scaledAABB;
		scaledAABB.min = glm::vec3(originalAABB.mMin.x, originalAABB.mMin.y, originalAABB.mMin.z) * scale + position;
		scaledAABB.max = glm::vec3(originalAABB.mMax.x, originalAABB.mMax.y, originalAABB.mMax.z) * scale + position;
		return scaledAABB;
	}

	

	void Scene::AABBDrawAll(Core::Graphics::OpenGLPipeline& pipeline)
	{
		if (!debugDrawALL) { return; }
		for (auto& obj : GetObjects()) {

			auto comps = obj->GetAllComponents();

			for (auto& comp : comps) {
				if (std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> modelComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp)) {

					auto glbModel = modelComp->GetModel();
					for (::Graphics::Primitives::Mesh& mesh : glbModel->Get()->getMeshes()) {

						auto meshABBB = mesh.getAABB();

						aabb scaledAABB = scaleAndPositionAABB(meshABBB, obj->GetScale(), obj->GetPosition());

						auto dim = scaledAABB.max - scaledAABB.min;
		

						Octree<::Graphics::Primitives::Mesh>::node* new_node = m_octree.create_node(scaledAABB);	
						pipeline.GetDebugSystem()->DrawAABB(obj->GetPosition(),dim, glm::vec4(1, 0.6, 0.2, 1), *pipeline.getCamera());

					}
				}
			}

		}
	}





	void Scene::CreateOctree(int levels, int sizebit){
		m_octree.set_root_size(1u << sizebit);
		m_octree.set_levels(levels);


		auto objects = GetObjects();

		for (auto& obj : objects) {

			auto comps = obj->GetAllComponents();

			for (auto& comp : comps) {
				if (std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> modelComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp)) {
					
					auto glbModel = modelComp->GetModel();
					for (::Graphics::Primitives::Mesh& mesh : glbModel->Get()->getMeshes()) {

						auto meshABBB = mesh.getAABB();

						aabb scaledAABB = scaleAndPositionAABB(meshABBB, obj->GetScale(), obj->GetPosition());

						Octree<::Graphics::Primitives::Mesh>::node* new_node = m_octree.create_node(scaledAABB);

						//check if it already had another octree::node list 
						if (mesh.m_octree_node) {
							if (mesh.m_octree_node == new_node)
								continue;
							else {
								EraseObjFromPrevNode(mesh, m_octree);
								mesh.m_octree_node = new_node;
								AddObjToNode(mesh, mesh.m_octree_node);
							}
						}
						else {
							mesh.m_octree_node = new_node;
							AddObjToNode(mesh, mesh.m_octree_node);
						}

						
					}



				}
			}

		}




	}



}