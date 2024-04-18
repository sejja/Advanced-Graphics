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
		class PointLight : public Lights::Light {
		public:
		#pragma region //Declaration
			struct PointLightData : public Light::BackedLightData {
			#pragma region //Methods
				float CalculateSphereOfInfluence() const noexcept;
			#pragma endregion

			#pragma region //Members
				float mRadius;
				float mInner;
				float mOutter;
				float mFallOff;
			#pragma endregion
			};
			#pragma endregion

			#pragma region //Constructor
			PointLight(const std::weak_ptr<Core::Object>& parent);
			#pragma endregion

			#pragma region //Methods
			void SetRadius(const float radius) noexcept;
			void SetInner(const float inner) noexcept;
			void SetOutter(const float outter) noexcept;
			void SetFallOff(const float falloff) noexcept;
			DONTDISCARD float GetRadius() const noexcept;
			DONTDISCARD float GetInner() const noexcept;
			DONTDISCARD float GetOutter() const noexcept;
			DONTDISCARD float GetFallOff() const noexcept;
			#pragma endregion
		};
	}
}

#endif