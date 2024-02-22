//
//	GBuffer.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright � 2024 . All Rights reserved
//


#ifndef _HDR_BUFFER__H_
#define _HDR_BUFFER__H_

#include "Core/ResourceManager.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "CommonDefines.h"
#include <glew.h>

namespace Core {
	namespace Graphics {
		class HDRBuffer {
		public:
			CLASS_EXCEPTION(HDRBuffer)
				HDRBuffer();
			~HDRBuffer();
			void Bind();
			void BindLightingShader();
			Asset<ShaderProgram> GetLightingShader();
			GLuint GetTextureHandle();
			void ClearBuffer();
			GLuint GetHandle();

		private:
			GLuint mBuffer;
			GLuint mTexture;
			Asset<ShaderProgram> mLightingShader;
		};
	}
}

#endif
