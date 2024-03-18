#include "Light.h"
#include <algorithm>

namespace Graphics {
	namespace Primitives {
		std::size_t Light::sLightReg = 0;

		Light::Light(std::weak_ptr<Core::Object> parent) : 
			Component(parent), mIndex(sLightReg++) {
		}

		Light::~Light() {
		}

		void Light::Update() {
			
		}

		void Light::SetPosition(const glm::vec3& position) {
			mData->mPosition = position;
		}

		glm::vec3 Light::GetPosition() const {
			return mData->mPosition;
		}
	}
}