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

		void SpotLight::SetDirection(glm::vec3 direction) {
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mDirection = direction;
		}

		void SpotLight::SetRadius(float radius)
		{
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mRadius = radius;
		}

		void SpotLight::SetInner(float inner)
		{
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mInner = inner;
		}

		void SpotLight::SetOuter(float outter)
		{
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mOutter = outter;
		}

		void SpotLight::SetFallOff(float falloff)
		{
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mFallOff = falloff;
		}

		void SpotLight::SetShadowCaster(bool shadow) {
			std::reinterpret_pointer_cast<SpotLightData>(mData)->mShadowCaster = shadow;
		}

		glm::vec3 SpotLight::GetDirection()
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mDirection;
		}

		float SpotLight::GetRadius()
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mRadius;
		}

		float SpotLight::GetInner()
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mInner;
		}

		float SpotLight::GetOutter()
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mOutter;
		}

		float SpotLight::GetFallOff()
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mFallOff;
		}

		bool SpotLight::GetShadowCasting()
		{
			return std::reinterpret_pointer_cast<SpotLightData>(mData)->mShadowCaster;
		}

		float SpotLight::SpotLightData::CalculateSphereOfInfluence() const {
			return 2 * mRadius;
		}
		void SpotLight::SpotLightData::GenerateShadowMap() {
			mShadowMap.Create();
			mShadowMap.CreateRenderTexture({1600, 900});
		}
	}
}