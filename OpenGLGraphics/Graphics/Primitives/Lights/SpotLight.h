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
		class SpotLight : public Lights::Light {
		#pragma region 
		public:
			struct SpotLightData : public Light::BackedLightData {
			#pragma region //Methods
				float CalculateSphereOfInfluence() const noexcept;
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
		#pragma endregion

		#pragma region //Methods
			void SetDirection(const glm::vec3& direction) noexcept;
			void SetRadius(const float radius) noexcept;
			void SetInner(const float inner) noexcept;
			void SetOuter(const float outter) noexcept;
			void SetFallOff(const float falloff) noexcept;
			void SetShadowCaster(const bool shadow) noexcept;
			DONTDISCARD glm::vec3 GetDirection() const noexcept;
			DONTDISCARD float GetRadius() const noexcept;
			DONTDISCARD float GetInner() const noexcept;
			DONTDISCARD float GetOutter() const noexcept;
			DONTDISCARD float GetFallOff() const noexcept;
			DONTDISCARD bool GetShadowCasting() const noexcept;
		#pragma endregion
		};
	}
}

#endif