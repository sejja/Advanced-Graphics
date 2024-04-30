#ifndef _CUBEMAPFRAMEBUFFER_H_
#define _CUBEMAPFRAMEBUFFER_H_

#include <glm.hpp>
#include <glew.h>
#include "../../Graphics/Architecture/GBuffer.h"
#include "../../Graphics/Primitives/GLBModel.h"
#include "../../Graphics/Primitives/ShaderProgram.h"
#include "../../Core/Assets/ResourceManager.h"
#include <functional>

namespace Core
{
	namespace Graphics
	{
		class CubemapFramebuffer
		{
		public:
			CubemapFramebuffer(const glm::u16vec2 dim);
			~CubemapFramebuffer();

			// Function to bind the cubemap
			void Bind();
			// Function to unbind the cubemap
			void Unbind();
			// Function to render to the cubemap
			void RenderToCubeMap(std::function<void(glm::mat4 projection, glm::mat4 view)> renderFunc);

		private:
			GLuint mBuffer, mPosition, mNormal, mAlbedoSpecular, mBrightness, mDepth, mDepthTexture;
			glm::u16vec2 mDimensions;
		};
	}
}

#endif // _CUBEMAPFRAMEBUFFER_H_
