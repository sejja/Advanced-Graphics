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
#include <Dependencies/Json/include/detail/input/parser.hpp>
#include "Core/AppWrapper.h"
#include "Core/Graphics/Pipeline.h"

namespace Core {
	// ------------------------------------------------------------------------
	/*! Create Scene
	*
	*   Creates a scene from a level file
	*/ // ---------------------------------------------------------------------
	void Scene::CreateScene(const std::string_view& file, std::function<void(const std::shared_ptr<Core::Object>& obj)> upload) {
		mParser.LoadDataFromFile(file.data());
		auto& resmg = Singleton<Core::Assets::ResourceManager>::Instance();

		std::for_each(std::execution::unseq, mParser.objects.begin(), mParser.objects.end(), [this, &upload, &resmg](const SceneParser::Transform& x) {
			std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
			obj->SetPosition(x.pos);
			obj->SetRotation(glm::radians(x.rot));
			obj->SetScale(x.sca);
			obj->SetName(x.name);
			obj->SetID(x.name);//temp , tiene que ser unico
			//obj->SetType() tiene que ser un enum

			std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::make_shared<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj);
			renderer->SetMesh(resmg.GetResource<::Graphics::Primitives::Model>(x.mesh.c_str()));

			if (x.name == "Sphere")
				renderer->SetShaderProgram(resmg.GetResource<Graphics::ShaderProgram>("Content/Shaders/Refractive.shader"));
			else
				renderer->SetShaderProgram(resmg.GetResource<Graphics::ShaderProgram>("Content/Shaders/DeferredGeometry.shader"));

			obj->AddComponent(std::move(renderer));
			upload(obj);
			mObjects.emplace_back(std::move(obj));
			});

		int i = 0;

		std::for_each(std::execution::seq, mParser.lights.begin(), mParser.lights.end(), [this, &i, &upload](const SceneParser::Light& x) {
			std::shared_ptr<Core::Object> obj = std::move(std::make_shared<Core::Object>());
			obj->SetPosition(x.pos);
			obj->SetRotation(glm::vec3(0.f, 0.f, 0.f));
			obj->SetScale({ 1.f, 1.f, 1.f });
			std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::move(std::make_shared<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj));
			std::shared_ptr<::Graphics::Primitives::Lights::Light> light;
			renderer->SetMesh(Singleton<Core::Assets::ResourceManager>::Instance().GetResource<::Graphics::Primitives::Model>("Content/Meshes/sphere_20_averaged.obj"));
			renderer->SetShaderProgram(Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/White.shader"));

			//TEMPORAL PARA SABER SI ES LUZ HASTA NUEVO LEVEL 
			obj->SetName(x.type + " Light_light");
			

			//If the light is a point light
			if (x.type == "POINT") {
				light = std::move(std::make_shared<::Graphics::Primitives::PointLight>(obj));
				std::reinterpret_pointer_cast<::Graphics::Primitives::PointLight>(light)->SetRadius(x.att.x);
				std::reinterpret_pointer_cast<::Graphics::Primitives::PointLight>(light)->SetFallOff(x.falloff);
			}

			//If the light is a directional light
			else if (x.type == "DIR") {
				light = std::move(std::make_shared<::Graphics::Primitives::Lights::DirectionalLight>(obj));
				std::reinterpret_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(light)->SetDirection(glm::normalize(x.dir));
			}

			//else, it's a spot light
			else {
				light = std::move(std::make_shared<::Graphics::Primitives::SpotLight>(obj));

				std::reinterpret_pointer_cast<::Graphics::Primitives::SpotLight>(light)->SetRadius(x.att.x);
				std::reinterpret_pointer_cast<::Graphics::Primitives::SpotLight>(light)->SetInner(x.inner);
				std::reinterpret_pointer_cast<::Graphics::Primitives::SpotLight>(light)->SetOuter(x.outer);
				std::reinterpret_pointer_cast<::Graphics::Primitives::SpotLight>(light)->SetFallOff(x.falloff);
				std::reinterpret_pointer_cast<::Graphics::Primitives::SpotLight>(light)->SetShadowCaster(1);
				std::reinterpret_pointer_cast<::Graphics::Primitives::SpotLight>(light)->SetDirection(x.dir);
			}

			light->SetPosition(glm::vec3(0.0f,0.0f,0.0f));
			light->SetColor(x.col);
			printf("COLOR: %f %f %f\n", x.col.x, x.col.y, x.col.z);


			std::weak_ptr< Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> weakrend = renderer;
			std::weak_ptr< ::Graphics::Primitives::Lights::Light> lightrend = light;

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

}