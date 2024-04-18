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
			struct PointLightData : public Light::BackedLightData {
				float mRadius;
				float mInner;
				float mOutter;
				float mFallOff;
				float CalculateSphereOfInfluence() const;
				void GenerateShadowMap();
			};
			PointLight(const std::weak_ptr<Core::Object>& parent);
			void SetRadius(float radius);
			void SetInner(float inner);
			void SetOutter(float outter);
			void SetFallOff(float falloff);
			float GetRadius();
			float GetInner();
			float GetOutter();
			float GetFallOff();
		};
	}
}

#endif