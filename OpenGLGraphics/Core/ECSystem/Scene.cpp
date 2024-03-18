//
//	Scene.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 03/03/24
//	Copyright © 2024. All Rights reserved
//

#include "Scene.h"
#include "Behaviors/AnimationComponent.h"
#include "Core/Pipeline.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"
#include "Graphics/Primitives/Lights/PointLight.h"
#include "Graphics/Primitives/Lights/SpotLight.h"
#include "Core/Singleton.h"
#include "Graphics/Primitives/Skybox.h"
#include "Graphics/Primitives/GLBModel.h"

namespace Core {
	// ------------------------------------------------------------------------
	/*! Create Scene
	*
	*   Creates a scene from a level file
	*/ // ---------------------------------------------------------------------
	void Scene::CreateScene(const std::string_view& file, const std::function<void(const std::shared_ptr<Core::Object>& obj)> upload) {
		mParser.LoadDataFromFile(file.data());
		auto& resmg = Singleton<ResourceManager>::Instance();

		std::for_each(std::execution::unseq, mParser.objects.begin(), mParser.objects.end(), [this, &upload, &resmg](const SceneParser::Transform& x) {
			std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
			obj->SetPosition(x.pos);
			obj->SetRotation(glm::radians(x.rot));
			obj->SetScale(x.sca);
			std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>> renderer = std::make_shared<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>>(obj);
			renderer->SetMesh(resmg.GetResource<::Graphics::Primitives::GLBModel>(x.mesh.c_str()));

			if (x.name == "suzanne_mesh")
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
			//std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>> renderer = std::move(std::make_shared<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>>(obj));
			std::shared_ptr<::Graphics::Primitives::Light> light;
			//renderer->SetMesh(Singleton<ResourceManager>::Instance().GetResource<::Graphics::Primitives::GLBModel>("Content/Meshes/sphere_20_averaged.obj"));

			//Switch by the light index (to switch behaviors)
			/*switch (i % 4) {
			case 0:
				renderer->SetShaderProgram(Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Blue.shader"));
				break;

			case 1:
				renderer->SetShaderProgram(Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/White.shader"));
				break;

			case 2:
				renderer->SetShaderProgram(Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/White.shader"));
				break;

			default:
				renderer->SetShaderProgram(Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/White.shader"));
				break;
			}*/

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
				((::Graphics::Primitives::DirectionalLight::DirectionalLightData*)light->mData)->mDirection = x.dir;
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

			//std::weak_ptr< Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>> weakrend = renderer;
			std::weak_ptr< ::Graphics::Primitives::Light> lightrend = light;

			//obj->AddComponent(std::move(weakrend));
			obj->AddComponent(std::move(lightrend));

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