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
#include <iostream>

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
				mData = std::shared_ptr<PointLightData>(sAlloc.New(), [](PointLightData* x) {sAlloc.deallocate(x); });
				Graphics::Architecture::LightPass::AddPointLight(std::reinterpret_pointer_cast<PointLightData>(mData));
				mData->mPosition = parent.lock()->GetPosition();
				std::reinterpret_pointer_cast<PointLightData>(mData)->GenerateShadowMap();
			}

			PointLight::~PointLight() {
				Graphics::Architecture::LightPass::RemovePointLight(std::reinterpret_pointer_cast<PointLightData>(mData));
			}

			void PointLight::PointLightData::GenerateShadowMap() {

				depthMapFBO.Create();

				const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024; //Esto lo he puesto un poco por que sí
				glGenTextures(1, &depthCubemap);
				glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
				for (unsigned int i = 0; i < 6; ++i) {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				}

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				//glActiveTexture(GL_TEXTURE12);
				//glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

				depthMapFBO.Bind();
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				depthMapFBO.Unbind();

				//if (glCheckFramebufferStatus(GL_FRAMEBUFFER))
			}
		}
	}
}