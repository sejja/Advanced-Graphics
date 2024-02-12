//
//	GBuffer.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright © 2021 . All Rights reserved
//


#ifndef _G_BUFFER__H_
#define _G_BUFFER__H_

#include "Core/ResourceManager.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "CommonDefines.h"
#include <glew.h>

namespace Core {
	namespace Graphics {
		class GBuffer {
		public:
			CLASS_EXCEPTION(GBuffer)
			GBuffer();
			~GBuffer();
			void Bind();
			void BindShader();
			Asset<ShaderProgram> GetGeometryShader();
			GLuint GetPositionTextureHandle();
			GLuint GetNormalTextureHandle();
			GLuint GetAlbedoTextureHandle();
			void ClearBuffer();

		private:
			GLuint mBuffer;
			GLuint mPosition, mNormal, mAlbedoSpecular;
			Asset<ShaderProgram> mGeometryShader;
		};
	}
}

#endif
