﻿//
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

namespace Graphics {
	namespace Architecture {
		namespace SSAO {
			class SSAOBuffer {
			public:
				SSAOBuffer(const glm::u16vec2& dim);
				void RenderAO(const GBuffer& gbuffer);

			private:
				GLuint mHandle, mColorBuffer, mBlurBuffer;
				SSAONoise mNoise;
			};
		}
	}
}

#endif