//
//	Texture.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _TEXTURE__H_
#define _TEXTURE__H_

#include "CommonDefines.h"
#include "Dependencies/GLEW/Include/glew.h"
#include <string>

namespace Core {
	namespace Graphics {
		class Texture {
		#pragma region //Declarations
		public:
			enum TextureType {
				eDiffuse,
				eNormal,
				eSpecular
			};
		#pragma endregion

		#pragma region //Constructors & Destructors
			Texture() noexcept;
			Texture(const std::string_view& filename) noexcept;
			~Texture() noexcept;
		#pragma endregion

		#pragma region //Functions
			inline void SetTextureType(const TextureType type) noexcept;
			void Bind() const noexcept;
			void LoadFromFile(const std::string_view& filename) noexcept;
		#pragma endregion

		#pragma region //Members
		private:
			GLuint mHandle;
			TextureType mType;
		#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Set Texture Type
		*
		*   Sets the Type of a certain texture
		*/ // --------------------------------------------------------------------
		void Texture::SetTextureType(const TextureType type) noexcept {
			mType = type;
		}
	}
}

#endif
