//
//	SSAOBuffer.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef _SSAO_BUFFER__H_
#define _SSAO_BUFFER__H_

#include <glew.h>
#include <glm.hpp>
#include "Graphics/Architecture/GBuffer.h"
#include "SSAONoise.h"
#include "Core/Assets/ResourceManager.h"

namespace Graphics {
	namespace Architecture {
		namespace SSAO {
			class SSAOBuffer {
			public:
				SSAOBuffer(const glm::u16vec2& dim);
				void RenderAO(const GBuffer& gbuffer);
				GLuint inline GetSSAOTexture() const noexcept;
				GLuint inline GetHandle() const;

			private:
				GLuint mHandle, mColorBuffer, mBlurBuffer, mBlurTexture;
				SSAONoise mNoise;
				std::vector<glm::vec3> mKernel;
				Core::Assets::Asset<Core::Graphics::ShaderProgram> mShaderSSAO, mBlurShader, mSSAOBlur;
			};

			GLuint SSAOBuffer::GetSSAOTexture() const noexcept {
				return mBlurTexture;
			}

			GLuint SSAOBuffer::GetHandle() const {
				return mHandle;
			}
		}
	}
}

#endif