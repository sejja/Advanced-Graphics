//
//	FrameBuffer.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 05/07/23
//	Copyright � 2023. All Rights reserved
//

#ifndef _FRAME_BUFFER__H_
#define _FRAME_BUFFER__H_

#include <glew.h>
#include "glm.hpp"
#include "CommonDefines.h"
#include "../Primitives/Texture.h"

namespace Core {
	namespace Graphics {
		class FrameBuffer {
		public:
		#pragma region //Declarations
			CLASS_EXCEPTION(FrameBuffer)
		#pragma endregion

		#pragma region //Constructors & Destructors
			FrameBuffer() noexcept;
			~FrameBuffer() noexcept;
		#pragma endregion

		#pragma region //Functions
			void Create();
			void CreateRenderTexture(glm::lowp_u16vec2 dimensions, bool depthonly = false, bool readable = true) ;
			void Bind();
			void inline Unbind() noexcept;
			void BindTexture(unsigned i = 0);
			void Clear(bool depthOnly = false);
			GLuint GetTextureHandle();
			GLuint GetHandle();
		#pragma endregion

		#pragma region //Members
		protected:
			GLuint mHandle;
			GLuint mTexture;
			GLuint mDepth;
			glm::lowp_u16vec2 mDimensions;
		#pragma endregion
		};
	}
}

#endif