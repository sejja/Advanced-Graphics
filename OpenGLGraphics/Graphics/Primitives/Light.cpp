#include "Light.h"
#include <algorithm>

namespace Graphics {
	namespace Primitives {
		std::size_t Light::sLightReg = 0;
		std::unordered_map<std::size_t, Light::BackedLightData> Light::sLightData;
	
		Light::Light(std::weak_ptr<Core::Object> parent) : 
			Component(parent), mIndex(sLightReg++) {
			sLightData.insert({ mIndex, mData });
		}

		Light::~Light() {
			sLightData.erase(mIndex);
		}

		void Light::Update() {
			if (!GetParent().expired()) {

				auto parent = GetParent().lock();
				sLightData[mIndex].mPosition = parent->GetPosition();
				sLightData[mIndex].mDirection = mData.mDirection;
				sLightData[mIndex].mAmbient = mData.mAmbient;
				sLightData[mIndex].mDiffuse = mData.mDiffuse;
				sLightData[mIndex].mSpecular = mData.mSpecular;
				sLightData[mIndex].mAttenuation = mData.mAttenuation;
				sLightData[mIndex].mInner = mData.mInner;
				sLightData[mIndex].mOutter = mData.mOutter;
				sLightData[mIndex].mFallOff = mData.mFallOff;
				sLightData[mIndex].mType = mData.mType;
			}
		}

		// ------------------------------------------------------------------------
		/*! Calculate Sphere of Influence
		*
		*   Gets the radious in which the light will affect the scene (with some bias)
		*   Formula might be found at: https://ogldev.org/www/tutorial36/threshold.jpg
		*/ //----------------------------------------------------------------------
		float Light::BackedLightData::CalculateSphereOfInfluence() const {
			glm::vec3 dif = mDiffuse * 255.f;
			glm::vec3 atte = mAttenuation * 255.f;
			glm::vec3 amb = mAmbient * 255.f;
			float MaxChannel = fmax(fmax(amb.x, amb.y), amb.z);
			float diffusechannel = fmax(fmax(dif.x, dif.y), dif.z);

			float ret = (-atte.y + sqrtf(std::max(atte.y * atte.y -
				4 * atte.z * (atte.z - 256 * MaxChannel * diffusechannel), 0.f)))
				/ (2 * atte.z);
			return ret;
		}
	}
}