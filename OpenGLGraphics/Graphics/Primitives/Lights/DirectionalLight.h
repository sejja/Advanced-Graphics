//
//	DirectionalLight.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "Light.h"

namespace Graphics {
	namespace Primitives {
		class DirectionalLight : public Light {
		public:
			struct DirectionalLightData : public Light::BackedLightData {
				glm::vec3 mDirection;
				glm::mat4 mShadowMatrix;
				Core::Graphics::FrameBuffer mShadowMap;
				float CalculateSphereOfInfluence() const override;
				void GenerateShadowMap() override;
			};
			DirectionalLight(const std::weak_ptr<Core::Object>& parent);
		};
	}
}

#endif