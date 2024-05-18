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
					float mTime = 0.0f;
					bool isFireLight;

					glm::vec4 fireParams = glm::vec4(0.15f, 0.25f, 1.0f,0.28f);
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

				DONTDISCARD bool& GetIsFireLight() const noexcept;
				void inline SetIsFireLight(bool isFireLight) const noexcept;

				DONTDISCARD glm::vec4& GetFireParams() const noexcept;
				void inline SetFireParams(glm::vec4 fireParams) const noexcept;

				void updateFireAnimation(float deltaTime) const noexcept;

				void inline Update() override {
					if (GetIsFireLight()) {
						updateFireAnimation(0.0167);
					}
				}


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

			inline DONTDISCARD bool& PointLight::GetIsFireLight() const noexcept
			{
				return std::reinterpret_pointer_cast<PointLightData>(mData)->isFireLight;
			}

			inline void PointLight::SetIsFireLight(bool isFireLight) const noexcept
			{
				std::reinterpret_pointer_cast<PointLightData>(mData)->isFireLight = isFireLight;
			}

			inline DONTDISCARD glm::vec4& PointLight::GetFireParams() const noexcept
			{
				return std::reinterpret_pointer_cast<PointLightData>(mData)->fireParams;
			}

			inline void PointLight::SetFireParams(glm::vec4 fireParams) const noexcept
			{
				std::reinterpret_pointer_cast<PointLightData>(mData)->fireParams = fireParams;
			}

			inline void PointLight::updateFireAnimation(float deltaTime) const noexcept{

				auto data = std::reinterpret_pointer_cast<PointLightData>(mData);

				data->mTime += deltaTime;

				float amplitude = std::reinterpret_pointer_cast<PointLightData>(mData)->fireParams.x;
				float frequency = std::reinterpret_pointer_cast<PointLightData>(mData)->fireParams.y;
				float baseFalloff = std::reinterpret_pointer_cast<PointLightData>(mData)->fireParams.z;

				float noiseAmplitude = 0.1f;  
				float noiseFrequency = std::reinterpret_pointer_cast<PointLightData>(mData)->fireParams[3];

				float sineComponent = amplitude * sin(frequency * data->mTime * 2.0f * 3.14159f);

				float noiseComponent = noiseAmplitude * sin(noiseFrequency/1000 * data->mTime * 2.0f * 3.14159f) * ((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f);

				float newFalloff = baseFalloff + sineComponent + noiseComponent;

				data->mFallOff = newFalloff;
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