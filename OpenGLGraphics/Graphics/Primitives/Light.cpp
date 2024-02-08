#include "Light.h"

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
	}
}