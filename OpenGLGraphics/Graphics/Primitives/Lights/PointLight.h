//
//	PointLight.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "Light.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			class PointLight : public Lights::Light {
			public:
#pragma region //Declaration
				struct PointLightData : public Light::BackedLightData {
#pragma region //Methods
					float inline CalculateSphereOfInfluence() const noexcept;
#pragma endregion

#pragma region //Members
					float mRadius;
					float mFallOff;
#pragma endregion
				};
#pragma endregion

#pragma region //Constructor
				PointLight(const std::weak_ptr<Core::Object>& parent);
				~PointLight();
#pragma endregion

#pragma region //Methods
				void inline SetRadius(const float radius) noexcept;
				void inline SetFallOff(const float falloff) noexcept;
				DONTDISCARD float inline GetRadius() const noexcept;
				DONTDISCARD float inline GetFallOff() const noexcept;
#pragma endregion
			};

			// ------------------------------------------------------------------------
			/*! Set Radius
			*
			*  Sets the light radius
			*/ //----------------------------------------------------------------------
			void PointLight::SetRadius(const float radius) noexcept {
				std::reinterpret_pointer_cast<PointLightData>(mData)->mRadius = radius;
			}

			// ------------------------------------------------------------------------
			/*! Set FallOff
			*
			*  Sets the light's FallOff
			*/ //----------------------------------------------------------------------
			void PointLight::SetFallOff(const float falloff) noexcept {
				std::reinterpret_pointer_cast<PointLightData>(mData)->mFallOff = falloff;
			}

			// ------------------------------------------------------------------------
			/*! Get Radius
			*
			*  Returns the light radius
			*/ //----------------------------------------------------------------------
			float PointLight::GetRadius() const noexcept {
				return std::reinterpret_pointer_cast<PointLightData>(mData)->mRadius;
			}

			// ------------------------------------------------------------------------
			/*! Get FallOff
			*
			*  Returns the light falloff
			*/ //----------------------------------------------------------------------
			float PointLight::GetFallOff() const noexcept {
				return std::reinterpret_pointer_cast<PointLightData>(mData)->mFallOff;
			}

			// ------------------------------------------------------------------------
			/*! Calculate Sphere of Influence
			*
			*  Calculates the Sphere of influence in which the light affect's some pixels
			*/ //----------------------------------------------------------------------
			float PointLight::PointLightData::CalculateSphereOfInfluence() const noexcept {
				return PI * mRadius * 2 / 3;
			}
		}
	}
}

#endif