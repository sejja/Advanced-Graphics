#include "Light.h"
#include <algorithm>
#include <iostream>

namespace Graphics {
	namespace Primitives {
		std::size_t Light::sLightReg = 0;

		Light::Light(std::weak_ptr<Core::Object> parent) : 
			Component(parent), mIndex(sLightReg++){
		}

		Light::~Light() {
		}

		void Light::Update() {
			
		}

		void Light::SetPosition(const glm::vec3& relativePos,const glm::vec3 objPos) {
			mData->mRelativePosition = relativePos;
			mData->mPosition = objPos + relativePos;
		}

		glm::vec3 Light::GetColor() const
		{
			return mData->mColor;
		}

		void Light::SetColor(const glm::vec3& color)
		{
			mData->mColor = color;
		}

		glm::vec3 Light::GetPosition() const {
			return mData->mRelativePosition;
		}

		

		void Light::BackedLightData::GenerateShadowMap()
		{
			std::cout << "UNDEFINED VIRTUAL FUNCTION GenerateShadowMap \n";
		}

		



	}
}