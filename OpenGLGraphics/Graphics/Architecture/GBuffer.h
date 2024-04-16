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

namespace Graphics {
	namespace Architecture {
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
			void BlitDepthBuffer(const GLuint buff) const;
			DONTDISCARD GLuint inline GetPositionTextureHandle() const;
			DONTDISCARD GLuint inline GetNormalTextureHandle() const;
			DONTDISCARD GLuint inline GetAlbedoTextureHandle() const;
			DONTDISCARD GLuint inline GetBrightnessTextureHandle() const;
			void ClearBuffer() const;
		#pragma endregion

		#pragma region //Members
		private:
			GLuint mBuffer, mPosition, mNormal, mAlbedoSpecular, mBrightness, mDepth;
			glm::u16vec2 mDimensions;
		#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Position Texture Handle
		*
		*   Returns the Handle for the position-stride texture
		*/ //----------------------------------------------------------------------
		GLuint GBuffer::GetPositionTextureHandle() const {
			return mPosition;
		}

		// ------------------------------------------------------------------------
		/*! get Normal Texture Handle
		*
		*   Returns the Handle for the normal-stride texture
		*/ //----------------------------------------------------------------------
		GLuint GBuffer::GetNormalTextureHandle() const {
			return mNormal;
		}

		// ------------------------------------------------------------------------
		/*! Get Albedo Texture Handle
		*
		*   Gets the Texture Handle for the Albedo-Specular texture
		*/ //----------------------------------------------------------------------
		GLuint GBuffer::GetAlbedoTextureHandle() const {
			return mAlbedoSpecular;
		}

		// ------------------------------------------------------------------------
		/*! Get Brightness Texture Handle
		*
		*   Returns the Handle for the brightness texture
		*/ //----------------------------------------------------------------------
		GLuint GBuffer::GetBrightnessTextureHandle() const {
			return mBrightness;
		}
	}
}

#endif
