#include "CubemapFramebuffer.h"
#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>


namespace Core
{
	namespace Graphics
	{
		CubemapFramebuffer::CubemapFramebuffer(int width, int height)
			: m_Width(width), m_Height(height)
		{
			// Create framebuffer object
			glGenFramebuffers(1, &m_FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

			// Generate cubemap texture
			glGenTextures(1, &m_TextureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
			for (unsigned int i = 0; i < 6; ++i)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			// Attach the texture to the framebuffer
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TextureID, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			// Check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Cubemap Framebuffer is not complete!" << std::endl;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		CubemapFramebuffer::~CubemapFramebuffer()
		{
			glDeleteTextures(1, &m_TextureID);
			glDeleteFramebuffers(1, &m_FBO);
		}

		void CubemapFramebuffer::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		}

		void CubemapFramebuffer::Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void CubemapFramebuffer::RenderToCubeMap(std::function<void(glm::mat4 projection, glm::mat4 view)> renderFunc)
		{
			// Store the current viewport dimensions
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

			// Set the viewport to the desired dimensions
			glViewport(0, 0, m_Width, m_Height);

			// Define the projection matrix
			glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
			// Define the view matrices for each face
			glm::mat4 views[6] = {
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
			};

			Bind();
			for (unsigned int i = 0; i < 6; ++i)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureID, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				renderFunc(projection, views[i]);
			}
			Unbind();

			// Restore the viewport to previous dimensions
			glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
		}
	}
}
