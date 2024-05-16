//
//	PointLight.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright � 2024 . All Rights reserved
//

#include "PointLight.h"
#include "Graphics/Architecture/LightPass.h"
#include "Core/Memory/PageAllocator.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			// ------------------------------------------------------------------------
			/*! Default Constructor
			*
			*  Constructor for the PointLight class
			*/ //----------------------------------------------------------------------
			PointLight::PointLight(const std::weak_ptr<Core::Object>& parent)
				: Light(parent) {
				static Core::Memory::PageAllocator<PointLightData> sAlloc;

				mData = std::shared_ptr<PointLightData>(sAlloc.New(), [](PointLightData* x) {sAlloc.deallocate(x);});
				Graphics::Architecture::LightPass::AddPointLight(std::reinterpret_pointer_cast<PointLightData>(mData));
				mData->mPosition = parent.lock()->GetPosition();
			}
			
			// ------------------------------------------------------------------------
			/*! Destructor
			*
			*  Remvoes the class from Point Light from the Light Pass
			*/ //----------------------------------------------------------------------
			PointLight::~PointLight() {
				Graphics::Architecture::LightPass::RemovePointLight(std::reinterpret_pointer_cast<PointLightData>(mData));
			}
		}
	}
}