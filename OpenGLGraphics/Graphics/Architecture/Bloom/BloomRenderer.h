//
//	BloomRenderer.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 1/03/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _BLOOM_RENDERER__H_
#define _BLOOM_RENDERER__H_

#include "BloomFrameBuffer.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "CommonDefines.h"

namespace Graphics{
	namespace Architecture {
		namespace Bloom {
            class BloomRenderer {
            public:
                CLASS_EXCEPTION(BloomRenderer)
                BloomRenderer();
                ~BloomRenderer();
                bool Init(unsigned int windowWidth, unsigned int windowHeight);
                void Destroy();
                void RenderBloomTexture(unsigned int srcTexture, float filterRadius, GLuint targetbuffer);
                unsigned int BloomTexture();

            private:
                void RenderDownsamples(unsigned int srcTexture);
                void RenderUpsamples(float filterRadius);

                bool mInit;
                BloomFBO mFBO;
                GLuint mScreenQuadVAO, mScreenQuadVBO;
                glm::ivec2 mSrcViewportSize;
                glm::vec2 mSrcViewportSizeFloat;
                Asset<Core::Graphics::ShaderProgram> mDownsampleShader;
                Asset<Core::Graphics::ShaderProgram> mUpsampleShader;
            };
		}
	}
}

#endif