//
//	SpotLight.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "SpotLight.h"
#include "Graphics/Architecture/LightPass.h"
#include "Core/Memory/PageAllocator.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			// ------------------------------------------------------------------------
			/*! Default Constructor
			*
			*  Constructor for the SpotLight class
			*/ //----------------------------------------------------------------------
			SpotLight::SpotLight(const std::weak_ptr<Core::Object>& parent)
				: Light(parent) {
				static Core::Memory::PageAllocator<SpotLightData> sAlloc;

				mData = std::shared_ptr<SpotLightData>(sAlloc.New(), [](SpotLightData* x) {sAlloc.deallocate(x); });
				Graphics::Architecture::LightPass::AddSpotLight(std::reinterpret_pointer_cast<SpotLightData>(mData));
				std::reinterpret_pointer_cast<SpotLightData>(mData)->GenerateShadowMap();
				mData->mPosition = parent.lock()->GetPosition();
			}

			// ------------------------------------------------------------------------
			/*! Destructor
			*
			*  Removes the Spotlight from the light pass
			*/ //----------------------------------------------------------------------
			SpotLight::~SpotLight() {
				Graphics::Architecture::LightPass::RemoveSpotLight(std::reinterpret_pointer_cast<SpotLightData>(mData));
			}

			// ------------------------------------------------------------------------
			/*! Generate Shadow Map
			*
			*  Generates the Shadow Map for texturing the shadows
			*/ //----------------------------------------------------------------------
			void SpotLight::SpotLightData::GenerateShadowMap() {
				mShadowMap.Create();
				mShadowMap.CreateRenderTexture({ 1600, 900 }, true, false);
			}
		}
	}
}