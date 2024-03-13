#include "Light.h"
#include <algorithm>

namespace Graphics {
	namespace Primitives {
		std::size_t Light::sLightReg = 0;
		std::unordered_map<std::size_t, Light::BackedLightData> Light::sLightData;
	
		Light::Light(std::weak_ptr<Core::Object> parent) : 
			Component(parent), mIndex(sLightReg++) {
			sLightData.insert({ mIndex, mData });
			GenerateShadowMap();
		}

		Light::~Light() {
			sLightData.erase(mIndex);
		}

		void Light::Update() {
			if (!GetParent().expired()) {

				auto parent = GetParent().lock();
				sLightData[mIndex].mPosition = parent->GetPosition();
				sLightData[mIndex].mDirection = mData.mDirection;
				sLightData[mIndex].mColor = mData.mColor;
				sLightData[mIndex].mRadius = mData.mRadius;
				sLightData[mIndex].mInner = mData.mInner;
				sLightData[mIndex].mOutter = mData.mOutter;
				sLightData[mIndex].mFallOff = mData.mFallOff;
				sLightData[mIndex].mType = mData.mType;
				sLightData[mIndex].mShadowCaster = mData.mShadowCaster;
			}
		}

		void Light::GenerateShadowMap() {
			sLightData[mIndex].mShadowMap.Create();
			sLightData[mIndex].mShadowMap.CreateRenderTexture({ 1078 * 4, 780 * 4 }, false);
		}

		// ------------------------------------------------------------------------
		/*! Calculate Sphere of Influence
		*
		*   Gets the radious in which the light will affect the scene (with some bias)
		*   Formula might be found at: https://ogldev.org/www/tutorial36/threshold.jpg
		*/ //----------------------------------------------------------------------
		float Light::BackedLightData::CalculateSphereOfInfluence() const {
			return mRadius * 2;
		}
	}
}