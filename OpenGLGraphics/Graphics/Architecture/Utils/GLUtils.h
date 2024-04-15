//
//	GLUtils.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 15/04/24
//	Copyright � 2024. All Rights reserved
//

#ifndef _GL_UTILS__H_
#define _GL_UTILS__H_

#include <glew.h>

namespace Graphics {
	namespace Architecture {
		namespace Utils {
			class GLUtils {
			#pragma region //Methods
			public:
				static void Init();
				static void RenderScreenQuad();
			#pragma endregion

			#pragma region //Members
			private:
				static GLuint quadVAO, quadVBO;
			#pragma endregion
			};
		}
	}
}

#endif