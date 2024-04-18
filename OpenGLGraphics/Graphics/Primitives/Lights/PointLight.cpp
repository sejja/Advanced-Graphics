//
//	PointLight.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "PointLight.h"
#include "Graphics/Architecture/LightPass.h"

namespace Graphics {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*  Constructor for the PointLight class
		*/ //----------------------------------------------------------------------
		PointLight::PointLight(const std::weak_ptr<Core::Object>& parent)
			: Light(parent) {
				mData = std::make_shared<PointLightData>();
				Graphics::Architecture::LightPass::AddPointLight(std::reinterpret_pointer_cast<PointLightData>(mData));
		}

		void PointLight::SetRadius(const float radius) noexcept {
			std::reinterpret_pointer_cast<PointLightData>(mData)->mRadius = radius;
		}

		void PointLight::SetInner(const float inner) noexcept {
			std::reinterpret_pointer_cast<PointLightData>(mData)->mInner = inner;
		}

		void PointLight::SetOutter(const float outter) noexcept {
			std::reinterpret_pointer_cast<PointLightData>(mData)->mOutter = outter;
		}

		void PointLight::SetFallOff(const float falloff) noexcept {
			std::reinterpret_pointer_cast<PointLightData>(mData)->mFallOff = falloff;
		}

		float PointLight::GetRadius() const noexcept {
			return std::reinterpret_pointer_cast<PointLightData>(mData)->mRadius;
		}

		float PointLight::GetInner() const noexcept {
			return std::reinterpret_pointer_cast<PointLightData>(mData)->mInner;
		}

		float PointLight::GetOutter() const noexcept {
			return std::reinterpret_pointer_cast<PointLightData>(mData)->mOutter;
		}

		float PointLight::GetFallOff() const noexcept {
			return std::reinterpret_pointer_cast<PointLightData>(mData)->mFallOff;
		}

		float PointLight::PointLightData::CalculateSphereOfInfluence() const noexcept {
			return 2 * mRadius;
		}
	}
}