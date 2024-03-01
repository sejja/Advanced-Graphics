//
//	BloomRenderer.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 1/03/24
//	Copyright © 2024. All Rights reserved
//

#include "BloomRenderer.h"

namespace Graphics {
	namespace Architecture {
		namespace Bloom {
            // ------------------------------------------------------------------------
            /*! Init
            *
            *   Initialized a Bloom Renderer that targets the whole window
            */ // ---------------------------------------------------------------------
            bool BloomRenderer::Init(unsigned int windowWidth, unsigned int windowHeight) {
                if (mInit)
                    throw BloomRendererException("Bloom Renderer was already 'Init'ed");
                mSrcViewportSize = glm::ivec2(windowWidth, windowHeight);
                mSrcViewportSizeFloat = glm::vec2((float)windowWidth, (float)windowHeight);

                // Framebuffer
                const unsigned int num_bloom_mips = 5; // Experiment with this value
                bool status = mFBO.Init(windowWidth, windowHeight, num_bloom_mips);

                //If FBO creation failed
                if (!status) 
                    throw BloomRendererException("Failed to initialize bloom FBO - cannot create bloom renderer!");

                // Shaders
                mDownsampleShader = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Downsampling.shader");
                mUpsampleShader = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Upsampling.shader");

                mInit = true;
                return true;
            }
		}
	}
}