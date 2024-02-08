//
//	Scene.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/03/23
//	Copyright © 2023. All Rights reserved
//

#include "Scene.h"
#include "Behaviors/AnimationComponent.h"
#include "Core/Pipeline.h"
#include "Graphics/Primitives/Light.h"
#include "Core/Singleton.h"
#include "Graphics/Primitives/Skybox.h"

namespace Core {
	SceneParser Scene::sParser;

	// ------------------------------------------------------------------------
	/*! Create Scene
	*
	*   Creates a scene from a level file
	*/ // ---------------------------------------------------------------------
	void Scene::CreateScene(const std::string_view& file, std::function<void(const std::shared_ptr<Core::Object>& obj)> upload) {
		sParser.LoadDataFromFile(file.data());
		auto& resmg = Singleton<ResourceManager>::Instance();

		std::for_each(std::execution::unseq, sParser.objects.begin(), sParser.objects.end(), [this, &upload, &resmg](const SceneParser::Transform& x) {
			std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
			obj->SetPosition(x.pos);
			obj->SetRotation(glm::radians(x.rot));
			obj->SetScale(x.sca);
			std::shared_ptr<Core::Graphics::ModelRenderer<Core::GraphicsAPIS::OpenGL>> renderer = std::make_shared<Core::Graphics::ModelRenderer<Core::GraphicsAPIS::OpenGL>>(obj);
			renderer->SetMesh(resmg.GetResource<Core::Graphics::Model>(x.mesh.c_str()));

			if (x.name == "suzanne_mesh")
				renderer->SetShaderProgram(resmg.GetResource<Graphics::ShaderProgram>("Content/Shaders/Refractive.shader"));
			else
				renderer->SetShaderProgram(resmg.GetResource<Graphics::ShaderProgram>("Content/Shaders/Textured.shader"));

			obj->AddComponent(std::move(renderer));
			upload(obj);
			mObjects.emplace_back(std::move(obj));
			});

		int i = 0;

		std::for_each(std::execution::seq, sParser.lights.begin(), sParser.lights.end(), [this, &i, &upload](const SceneParser::Light& x) {
			std::shared_ptr<Core::Object> obj = std::move(std::make_shared<Core::Object>());
			obj->SetPosition(x.pos);
			obj->SetRotation(glm::vec3(0.f, 0.f, 0.f));
			obj->SetScale({ 1.f, 1.f, 1.f });
			std::shared_ptr<Core::Graphics::ModelRenderer<Core::GraphicsAPIS::OpenGL>> renderer = std::move(std::make_shared<Core::Graphics::ModelRenderer<Core::GraphicsAPIS::OpenGL>>(obj));
			std::shared_ptr<::Graphics::Primitives::Light> light = std::move(std::make_shared<::Graphics::Primitives::Light>(obj));
			renderer->SetMesh(Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Model>("Content/Meshes/sphere_20_averaged.obj"));
			renderer->SetShaderProgram(Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/White.shader"));
			light->SetPosition(x.pos);
			light->mData.mDirection = x.dir;
			light->mData.mAmbient = glm::vec3(x.amb, x.amb, x.amb);
			light->mData.mDiffuse = x.col;
			light->SetSpecular(x.col);
			light->SetAttenuation(x.att);
			light->mData.mInner = x.inner;
			light->mData.mOutter = x.outer;
			light->mData.mFallOff = x.falloff;

			//If the light is a point light
			if (x.type == "POINT") light->mData.mType = ::Graphics::Primitives::Light::LightType::Point;

			//If the light is a directional light
			else if (x.type == "DIR")
				light->mData.mType = ::Graphics::Primitives::Light::LightType::Directional;

			//else, it's a spot light
			else light->mData.mType = ::Graphics::Primitives::Light::LightType::Spot;

			std::weak_ptr< Core::Graphics::ModelRenderer<Core::GraphicsAPIS::OpenGL>> weakrend = renderer;
			std::weak_ptr< ::Graphics::Primitives::Light> lightrend = light;

			obj->AddComponent(std::move(weakrend));
			obj->AddComponent(std::move(lightrend));

			//Switch by the light index (to switch behaviors)
			switch (i % 4) {
			case 0:
				obj->AddComponent(std::move((std::make_shared<Behaviors::AnimationComponent>(obj))));
				break;

			case 1:
				obj->AddComponent(std::move((std::make_shared<Behaviors::AnimationComponent2>(obj))));
				break;

			case 2:
				obj->AddComponent(std::move((std::make_shared<Behaviors::AnimationComponent3>(obj))));
				break;

			default:
				obj->AddComponent(std::move((std::make_shared<Behaviors::AnimationComponent4>(obj))));
				break;
			}

			i++;
			upload(obj);
			mObjects.emplace_back(std::move(obj));
			});

		std::shared_ptr<Core::Object> sky = std::move(std::make_shared<Core::Object>());
		std::shared_ptr<Core::Graphics::Skybox> skycomp = std::make_shared<Core::Graphics::Skybox>(sky);
		skycomp->CreateCubeMap();
		sky->AddComponent(std::move(skycomp));
		mObjects.emplace_back(sky);

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