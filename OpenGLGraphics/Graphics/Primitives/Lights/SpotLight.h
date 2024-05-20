//
//	SpotLight.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef _SPOT_LIHGT_H_
#define _SPOT_LIHGT_H_

#include "Light.h"
#include "Graphics/Tools/FrameBuffer.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			class SpotLight : public Lights::Light {
#pragma region 
			public:
				struct SpotLightData : public Light::BackedLightData {
#pragma region //Methods
					float inline CalculateSphereOfInfluence() const noexcept;
					void GenerateShadowMap();
#pragma endregion

#pragma region //Members
					glm::vec3 mDirection;
					float mRadius;
					float mInner;
					float mOutter;
					float mFallOff;
					glm::mat4 mShadowMatrix;
					Core::Graphics::FrameBuffer mShadowMap;
#pragma endregion
				};
#pragma endregion

#pragma region //Constructor
				SpotLight(const std::weak_ptr<Core::Object>& parent);
				~SpotLight();
#pragma endregion

#pragma region //Methods
				void inline SetDirection(const glm::vec3& direction) noexcept;
				void inline SetRadius(const float radius) noexcept;
				void inline SetInner(const float inner) noexcept;
				void inline SetOuter(const float outter) noexcept;
				void inline SetFallOff(const float falloff) noexcept;
				void inline SetShadowCaster(const bool shadow) noexcept;
				DONTDISCARD glm::vec3 inline GetDirection() const noexcept;
				DONTDISCARD float inline GetRadius() const noexcept;
				DONTDISCARD float inline GetInner() const noexcept;
				DONTDISCARD float inline GetOutter() const noexcept;
				DONTDISCARD float inline GetFallOff() const noexcept;
				DONTDISCARD bool inline GetShadowCasting() const noexcept;
				
#pragma endregion
			};

			// ------------------------------------------------------------------------
			/*! Set Direction
			*
			*  Sets the Direction of the SpotLight
			*/ //----------------------------------------------------------------------
			void SpotLight::SetDirection(const glm::vec3& direction) noexcept {
				std::reinterpret_pointer_cast<SpotLightData>(mData)->mDirection = direction;
			}

			// ------------------------------------------------------------------------
			/*! Set Radius
			*
			*  Sets the Spotlight's radius
			*/ //----------------------------------------------------------------------
			void SpotLight::SetRadius(float radius) noexcept {
				std::reinterpret_pointer_cast<SpotLightData>(mData)->mRadius = radius;
			}

			// ------------------------------------------------------------------------
			/*! Set Inner
			*
			*  Sets the Inner angle of the light
			*/ //----------------------------------------------------------------------
			void SpotLight::SetInner(float inner) noexcept {
				std::reinterpret_pointer_cast<SpotLightData>(mData)->mInner = inner;
			}

			// ------------------------------------------------------------------------
			/*! Set Outer
			*
			*  Sets the Outer angle of the light
			*/ //----------------------------------------------------------------------
			void SpotLight::SetOuter(float outter) noexcept {
				std::reinterpret_pointer_cast<SpotLightData>(mData)->mOutter = outter;
			}

			// ------------------------------------------------------------------------
			/*! Set Fall Off
			*
			*  Sets the Fall Off of the Spotlight
			*/ //----------------------------------------------------------------------
			void SpotLight::SetFallOff(float falloff) noexcept {
				std::reinterpret_pointer_cast<SpotLightData>(mData)->mFallOff = falloff;
			}

			// ------------------------------------------------------------------------
			/*! Set Shadow Caster
			*
			*  Sets wether this light casts shadows or not
			*/ //----------------------------------------------------------------------
			void SpotLight::SetShadowCaster(const bool shadow) noexcept {
				std::reinterpret_pointer_cast<SpotLightData>(mData)->mShadowCaster = shadow;
			}

			// ------------------------------------------------------------------------
			/*! Get Direction
			*
			*  Returns the direction of the light
			*/ //----------------------------------------------------------------------
			glm::vec3 SpotLight::GetDirection() const noexcept {
				return std::reinterpret_pointer_cast<SpotLightData>(mData)->mDirection;
			}

			// ------------------------------------------------------------------------
			/*! Get Radius
			*
			*  Returns the radius of the light
			*/ //----------------------------------------------------------------------
			float SpotLight::GetRadius() const noexcept {
				return std::reinterpret_pointer_cast<SpotLightData>(mData)->mRadius;
			}

			// ------------------------------------------------------------------------
			/*! Get Inner Angle
			*
			*  Returns the light inner angle
			*/ //----------------------------------------------------------------------
			float SpotLight::GetInner() const noexcept {
				return std::reinterpret_pointer_cast<SpotLightData>(mData)->mInner;
			}

			// ------------------------------------------------------------------------
			/*! Get Outter Angle
			*
			*  Returns the outer angle of the light
			*/ //----------------------------------------------------------------------
			float SpotLight::GetOutter() const noexcept {
				return std::reinterpret_pointer_cast<SpotLightData>(mData)->mOutter;
			}

			// ------------------------------------------------------------------------
			/*! Get Fall Off
			*
			*  Returns the Fall Off of the light
			*/ //----------------------------------------------------------------------
			float SpotLight::GetFallOff() const noexcept {
				return std::reinterpret_pointer_cast<SpotLightData>(mData)->mFallOff;
			}

			// ------------------------------------------------------------------------
			/*! Get Shadow Casting
			*
			*  Returns wether a light casts shadow or not
			*/ //----------------------------------------------------------------------
			bool SpotLight::GetShadowCasting() const noexcept {
				return std::reinterpret_pointer_cast<SpotLightData>(mData)->mShadowCaster;
			}

			// ------------------------------------------------------------------------
			/*! Calculate Sphere of Influence
			*
			*  Constructor for the SpotLight class
			*/ //----------------------------------------------------------------------
			float SpotLight::SpotLightData::CalculateSphereOfInfluence() const noexcept {
				return PI * mRadius;
			}
		}
	}
}

#endif