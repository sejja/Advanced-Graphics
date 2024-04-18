//
//	SpotLight.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "SpotLight.h"
#include "Graphics/Architecture/LightPass.h"

namespace Graphics {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*  Constructor for the SpotLight class
		*/ //----------------------------------------------------------------------
		SpotLight::SpotLight(const std::weak_ptr<Core::Object>& parent)
			: Light(parent) {
				mData = std::make_shared<SpotLightData>();
				Graphics::Architecture::LightPass::AddSpotLight(std::reinterpret_pointer_cast<SpotLightData>(mData));
				std::reinterpret_pointer_cast<SpotLightData>(mData)->GenerateShadowMap();
		}

		void SpotLight::SetDirection(const glm::vec3& direction) noexcept {
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mDirection = direction;
		}

		void SpotLight::SetRadius(float radius) noexcept
		{
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mRadius = radius;
		}

		void SpotLight::SetInner(float inner) noexcept
		{
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mInner = inner;
		}

		void SpotLight::SetOuter(float outter) noexcept
		{
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mOutter = outter;
		}

		void SpotLight::SetFallOff(float falloff) noexcept
		{
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mFallOff = falloff;
		}

		void SpotLight::SetShadowCaster(bool shadow) noexcept {
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mShadowCaster = shadow;
		}

		glm::vec3 SpotLight::GetDirection() const noexcept
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mDirection;
		}

		float SpotLight::GetRadius() const noexcept
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mRadius;
		}

		float SpotLight::GetInner() const noexcept
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mInner;
		}

		float SpotLight::GetOutter() const noexcept
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mOutter;
		}

		float SpotLight::GetFallOff() const noexcept
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mFallOff;
		}

		bool SpotLight::GetShadowCasting() const noexcept
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mShadowCaster;
		}

		float SpotLight::SpotLightData::CalculateSphereOfInfluence() const noexcept {
			return 2 * mRadius;
		}
		void SpotLight::SpotLightData::GenerateShadowMap() {
			mShadowMap.Create();
			mShadowMap.CreateRenderTexture({1600, 900});
		}
	}
}