//
//	LightPass.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 13/03/24
//	Copyright � 2024. All Rights reserved
//

#ifndef _LIGHT_PASS__H_
#define _LIGHT_PASS__H_

#include <glm.hpp>
#include "Graphics/Architecture/GBuffer.h"
#include "Graphics/Primitives/GLBModel.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Core/Assets/ResourceManager.h"
#include "Graphics/Primitives/Lights/Light.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"
#include "Graphics/Primitives/Lights/SpotLight.h"
#include "Graphics/Primitives/Lights/PointLight.h"
#include <functional>

namespace Graphics {
	namespace Architecture {
		class LightPass {
		#pragma region //Declarations
		template<typename T>
		using lightmap = std::unordered_map<std::size_t, std::shared_ptr<T>>;
		#pragma endregion

		#pragma region //Constructor
		public:
			LightPass();
		#pragma endregion

		#pragma region //Methods
			void RenderShadowMaps(const glm::u16vec2 dim, const glm::mat4& camview, const std::function<void(Core::Graphics::ShaderProgram*)>& rend_func) const;
			void RenderLights(const glm::u16vec2 dim, const GBuffer& gBuffer) const;
			void StencilPass(const glm::vec3& pos, const float sphere) const;
			static void inline AddPointLight(const std::size_t idx, const std::shared_ptr<Graphics::Primitives::PointLight::PointLightData>& data);
			static void inline AddSpotLight(const std::size_t idx, const std::shared_ptr<Graphics::Primitives::SpotLight::SpotLightData>& data);
			static void inline AddDirectionalLight(const std::size_t idx, const std::shared_ptr<Graphics::Primitives::DirectionalLight::DirectionalLightData>& data);
			static void inline RemovePointLight(const std::size_t idx);
			static void inline RemoveSpotLight(const std::size_t idx);
			static void inline RemoveDirectionalLight(const std::size_t idx);
		#pragma endregion

		#pragma region //Members
		private:
			Core::Assets::Asset<::Graphics::Primitives::GLBModel> mLightSphere;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mLightSphereShader;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mDirectionalShader;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mPointShader;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mSpotShader;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mShadowShader;
			static lightmap< Graphics::Primitives::DirectionalLight::DirectionalLightData> sDirectionalLightData;
			static lightmap< Graphics::Primitives::SpotLight::SpotLightData> sSpotLightData;
			static lightmap< Graphics::Primitives::PointLight::PointLightData> sPointLightData;
		#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Add Point Light
		*
		*   Adds a Point light data to the light pass pipeline
		*/ //----------------------------------------------------------------------
		void LightPass::AddPointLight(const std::size_t idx, const std::shared_ptr<Graphics::Primitives::PointLight::PointLightData>& data) {
			sPointLightData.insert(std::make_pair(idx, data));
		}

		// ------------------------------------------------------------------------
		/*! Add Spot Light
		*
		*   Adds a Spot light data to the light pass pipeline
		*/ //----------------------------------------------------------------------
		void LightPass::AddSpotLight(const std::size_t idx, const std::shared_ptr<Graphics::Primitives::SpotLight::SpotLightData>& data) {
			sSpotLightData.insert(std::make_pair(idx, data));
		}

		// ------------------------------------------------------------------------
		/*! Add Driectional Light
		*
		*   Adds a Directional light data to the light pass pipeline
		*/ //----------------------------------------------------------------------
		void LightPass::AddDirectionalLight(const std::size_t idx, const std::shared_ptr<Graphics::Primitives::DirectionalLight::DirectionalLightData>& data) {
			sDirectionalLightData.insert(std::make_pair(idx, data));
		}

		// ------------------------------------------------------------------------
		/*! Remove Point Light
		*
		*   Remove Point light data from the light pass pipeline referenced by index
		*/ //----------------------------------------------------------------------
		void LightPass::RemovePointLight(const std::size_t idx) {
			sPointLightData.erase(idx);
		}

		// ------------------------------------------------------------------------
		/*! Remove Spot Light
		*
		*   Remove Spot light data from the light pass pipeline referenced by index
		*/ //----------------------------------------------------------------------
		void LightPass::RemoveSpotLight(const std::size_t idx) {
			sSpotLightData.erase(idx);
		}

		// ------------------------------------------------------------------------
		/*! Remove Directional Light
		*
		*   Remove Directional light data from the light pass pipeline referenced by index
		*/ //----------------------------------------------------------------------
		void LightPass::RemoveDirectionalLight(const std::size_t idx) {
			sDirectionalLightData.erase(idx);
		}
	}
}

#endif