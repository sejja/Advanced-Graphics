#pragma once

#include "glm.hpp"
#include "vec3.hpp"
#include "Core/ECSystem/Component.h"
#include <unordered_map>
#include <memory>
#include "Graphics/Tools/FrameBuffer.h"

namespace Graphics {
	namespace Primitives {
		class Light : public Core::Component {
		public:
			enum class LightType {
				Point = 0,
				Spot = 1,
				Directional = 2
			};

			struct BackedLightData {
				glm::vec3 mPosition;
				glm::vec3 mDirection;
				glm::vec3 mColor;
				float mRadius;
				float mInner;
				float mOutter;
				float mFallOff;
				int mShadowCaster;
				LightType mType;
				float CalculateSphereOfInfluence() const;
				Core::Graphics::FrameBuffer mShadowMap;
			};
		
			Light(std::weak_ptr<Core::Object> parent);
			~Light();

			void GenerateShadowMap();
			void Update() override;
			const glm::vec3& GetPosition() const { return mData.mPosition; }

			void SetPosition(const glm::vec3& position) { mData.mPosition = position; }

			static std::unordered_map<std::size_t, BackedLightData> sLightData;
			static std::size_t sLightReg;

			std::size_t mIndex;
			BackedLightData mData;
		};
	}
}