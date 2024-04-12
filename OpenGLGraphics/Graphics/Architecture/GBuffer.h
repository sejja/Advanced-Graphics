//
//	GBuffer.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright � 2024 . All Rights reserved
//


#ifndef _G_BUFFER__H_
#define _G_BUFFER__H_

#include <glew.h>
#include "Core/Assets/ResourceManager.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "CommonDefines.h"

namespace Core {
	namespace Graphics {
		class GBuffer {
		#pragma region //Declarations
		public:
			CLASS_EXCEPTION(GBuffer)
		#pragma endregion

		#pragma region //Constructors & Destructors
			GBuffer(const glm::u16vec2 dim);
			~GBuffer();
		#pragma endregion

		#pragma region //Methods
			void Bind() const;
			void BlitDepthBuffer(GLuint buff = NULL) const;
			GLuint GetPositionTextureHandle() const;
			GLuint GetNormalTextureHandle() const;
			GLuint GetAlbedoTextureHandle() const;
			GLuint GetBrightnessTextureHandle() const;
			void ClearBuffer() const;
		#pragma endregion

		#pragma region //Members
		private:
			GLuint mBuffer, mPosition, mNormal, mAlbedoSpecular, mBrightness;
		#pragma endregion
		};
	}
}

#endif
