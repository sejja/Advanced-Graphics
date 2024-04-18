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
		public:
			struct SpotLightData : public Light::BackedLightData {
				glm::vec3 mDirection;
				float mRadius;
				float mInner;
				float mOutter;
				float mFallOff;
				glm::mat4 mShadowMatrix;
				Core::Graphics::FrameBuffer mShadowMap;
				float CalculateSphereOfInfluence() const;
				void GenerateShadowMap();
			};

			
			SpotLight(const std::weak_ptr<Core::Object>& parent);
		};
	}
}

#endif