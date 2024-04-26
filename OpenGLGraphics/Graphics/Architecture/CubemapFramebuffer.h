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
			CubemapFramebuffer(int width, int height);
			~CubemapFramebuffer();

			// Function to bind the cubemap
			void Bind();
			// Function to unbind the cubemap
			void Unbind();
			// Function to render to the cubemap
			void RenderToCubeMap(std::function<void(glm::mat4 projection, glm::mat4 view)> renderFunc);

			// Getters
			GLuint GetTextureID() const { return m_TextureID; }

		private:
			GLuint m_FBO; // Framebuffer object
			GLuint m_TextureID; // Cubemap texture ID
			int m_Width, m_Height;
		};
	}
}

#endif // _CUBEMAPFRAMEBUFFER_H_
