#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "Core/ECSystem/Component.h"
#include <unordered_map>
#include <memory>

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
				glm::vec3 mAmbient;
				glm::vec3 mDiffuse;
				glm::vec3 mSpecular;
				glm::vec3 mAttenuation;
				float mInner;
				float mOutter;
				float mFallOff;
				LightType mType;
			};
		
			Light(std::weak_ptr<Core::Object> parent);
			~Light();

			void Update() override;
			const glm::vec3& GetPosition() const { return mData.mPosition; }
			const glm::vec3& GetmSpecular() const { return mData.mSpecular; }
			glm::vec3 GetAttenuation() const { return mData.mAttenuation; }

			void SetPosition(const glm::vec3& position) { mData.mPosition = position; }
			void SetSpecular(const glm::vec3& color) { mData.mSpecular = color; }
			void SetAttenuation(glm::vec3 intensity) { mData.mAttenuation = intensity; }

			static std::unordered_map<std::size_t, BackedLightData> sLightData;
			static std::size_t sLightReg;

			std::size_t mIndex;
			BackedLightData mData;
		};
	}
}