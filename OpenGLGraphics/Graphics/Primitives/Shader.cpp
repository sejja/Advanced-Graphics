//
//	Shader.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Windows.h>
#include <fstream>
#include <sstream>
#include <glew.h>
#include "Shader.h"

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a Shader
		*/ // --------------------------------------------------------------------
		Shader::Shader() noexcept :
			mHandle(NULL), ShaderType(EType::Vertex) {
		}

		// ------------------------------------------------------------------------
		/*! Destrutor
		*
		*   Frees a Shader
		*/ // --------------------------------------------------------------------
		Shader::~Shader() noexcept {
			if(mHandle) glDeleteShader(mHandle);
		}

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Shader given a file
		*/ // --------------------------------------------------------------------
		Shader::Shader(const std::string_view& filename, const EType type) :
			ShaderType(type), mHandle(NULL) {
			Compile(LoadSource(filename));
		}

		// ------------------------------------------------------------------------
		/*! Skip Byte Order Mark
		*
		*   To Fix Encoding Issues, we remove possible BOMs
		*/ // --------------------------------------------------------------------
		void SkipBOM(std::ifstream& in) {
			char test[3] = { 0 };
			in.read(test, 3);
			if ((unsigned char)test[0] == 0xEF && (unsigned char)test[1] == 0xBB && (unsigned char)test[2] == 0xBF)
				return;
			in.seekg(0);
		}

		// ------------------------------------------------------------------------
		/*! Load Source
		*
		*   Loads a Source for our Shader
		*/ // --------------------------------------------------------------------
		char* Shader::LoadSource(const std::string_view& filename) const noexcept {
			std::ifstream shaderFile(filename.data());
			std::stringstream shaderSource;
			char* source;

			SkipBOM(shaderFile);
			shaderSource << shaderFile.rdbuf();

			//If we could allocate the string
			if (source = reinterpret_cast<char*>(malloc(strlen(shaderSource.str().c_str()) + 1)))
				strcpy(source, shaderSource.str().c_str());
			else
				return nullptr;
			return source;
		}

		// ------------------------------------------------------------------------
		/*! Compile
		*
		*   Compiles the Shader
		*/ // --------------------------------------------------------------------
		void Shader::Compile(char* source) {
			//If there is a valid source file
			if (source && strlen(source)) {
				CreateDeviceShader();
				glShaderSource(static_cast<GLuint>(mHandle), 1, &source, NULL);
				glCompileShader(static_cast<GLuint>(mHandle));

				// sanity check
				GLint result;
				glGetShaderiv(static_cast<GLuint>(mHandle), GL_COMPILE_STATUS, &result);

#ifdef _DEBUG
				//If there has been errors during compilation
				if (!result) throw ShaderException("Couldn't compile shader");
#endif
				free(source);
			} else 
				throw ShaderException("Can't compile empty shader");
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Type
		*
		*   Sets the type of the shader
		*/ // --------------------------------------------------------------------
		void Shader::SetShaderType(Shader::EType shaderType, bool createDeviceShader) {
			ShaderType = shaderType;

			//If we need to create a device
			if (createDeviceShader)
				CreateDeviceShader();
		}
	}
}