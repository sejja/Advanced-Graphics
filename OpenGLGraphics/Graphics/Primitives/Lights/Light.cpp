#include "Light.h"
#include <algorithm>
#include <iostream>

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			Light::Light(const std::weak_ptr<Core::Object>& parent) :
				Component(parent) {
			}

			Light::~Light() {
			}

			void Light::SetPosition(const glm::vec3& relativePos) noexcept {
				mData->mPosition = GetParent().lock()->GetPosition() + relativePos;
			}

			glm::vec3 Light::GetColor() const noexcept
			{
				return mData->mColor;
			}

			void Light::SetColor(const glm::vec3& color) noexcept
			{
				mData->mColor = color;
			}

			void Light::SetData(const std::weak_ptr<BackedLightData>& data) {
				mData = data.lock();
			}

			std::weak_ptr<Light::BackedLightData> Light::GetData() const noexcept {
				return mData;
			}

			glm::vec3 Light::GetPosition() const noexcept {
				return mData->mPosition - GetParent().lock()->GetPosition();
			}
		}
	}
}