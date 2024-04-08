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
			struct BackedLightData {
				glm::vec3 mPosition;
				glm::vec3 mRelativePosition;
				glm::vec3 mColor;
				int mShadowCaster;
				virtual float CalculateSphereOfInfluence() const = 0;
				virtual void GenerateShadowMap();
			};

			Light(std::weak_ptr<Core::Object> parent);
			~Light();

			void Update() override;
			glm::vec3 GetPosition() const;
			void SetPosition(const glm::vec3& position, const glm::vec3 objPos);

			glm::vec3 GetColor() const;
			void SetColor(const glm::vec3& color);

			static std::size_t sLightReg;
			
			std::size_t mIndex;
			BackedLightData* mData;
	
	private:
		
		};
	}
}