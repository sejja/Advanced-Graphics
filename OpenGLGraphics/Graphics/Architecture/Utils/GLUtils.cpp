//
//	GLUtils.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 15/04/24
//	Copyright � 2024. All Rights reserved
//

#include "GLUtils.h"

namespace Graphics {
	namespace Architecture {
		namespace Utils {
			GLuint GLUtils::quadVAO;
			GLuint GLUtils::quadVBO;

			// ------------------------------------------------------------------------
			/*! Init
			*
			*   Initializes useful stuff
			*/ //----------------------------------------------------------------------
			void GLUtils::Init() {
				float quadVertices[] = {
					// positions        // texture Coords
					-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
					 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				};
				// setup plane VAO
				glGenVertexArrays(1, &quadVAO);
				glGenBuffers(1, &quadVBO);
				glBindVertexArray(quadVAO);
				glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			}

			// ------------------------------------------------------------------------
			/*! Render Screen Quad
			*
			*   Renders a quad that covers the full screen
			*/ //----------------------------------------------------------------------
			void GLUtils::RenderScreenQuad() {
				glBindVertexArray(quadVAO);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
		}
	}
}