//
//	Shader.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _SHADER__H_
#define _SHADER__H_

#include "CommonDefines.h"
#include <glew.h>

namespace Core {
	namespace Graphics {
		class  Shader {
		public:
		#pragma region //Declarations
			CLASS_EXCEPTION(Shader)
			enum class EType : unsigned char {
				Vertex,
				Fragment
			};
		#pragma endregion

		#pragma region //Constructors & Destructor
			Shader() noexcept;
			Shader(const std::string_view& filename, const EType type);
			~Shader() noexcept;
		#pragma endregion

		#pragma region //Methods
			void SetShaderType(const EType type, const bool createDeviceShader = false);
			inline GLuint GetGLHandle() const noexcept;

		private:
			char* LoadSource(const std::string_view& filename) const noexcept;
			void Compile(char* source);
			void inline CreateDeviceShader() noexcept;
		#pragma endregion
		
		#pragma region //Members
			GLuint mHandle;
			EType ShaderType;
		#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get GL Handle
		*
		*   Returns the OpenGL Handle of this shader
		*/ // ---------------------------------------------------------------------
		GLuint Shader::GetGLHandle() const noexcept {
			return mHandle;
		}

		// ------------------------------------------------------------------------
		/*! Create Device Shader
		*
		*   Creates a Device for the Shader
		*/ // --------------------------------------------------------------------
		void Shader::CreateDeviceShader() noexcept {
			mHandle = glCreateShader(ShaderType == EType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		}
	}
}

#endif
