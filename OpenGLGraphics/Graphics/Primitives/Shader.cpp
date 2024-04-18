//
//	Shader.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2024 . All Rights reserved
//

#include <Windows.h>
#include <fstream>
#include <sstream>
#include <glew.h>
#include "Shader.h"
#include <iostream>

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

			//Asign Uniform Buffers if the shader creation was correct
			if(mHandle)
				glUniformBlockBinding(mHandle, 
					glGetUniformBlockIndex(mHandle, "UniformBuffer"), 
					NULL);
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

		void Shader::ReloadShaderSPIRV(const std::string_view& filename, EType type)
		{
			// Delete existing shader
			glDeleteShader(mHandle);

			// Load filename in binary
			std::ifstream file(filename.data(), std::ios::binary);
			if (!file.is_open()) {
				std::cerr << "Failed to open shader file: " << filename << std::endl;
				return;
			}

			std::vector<char> vs_buf((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

			GLuint sid;
			if (type == EType::Fragment)
				sid = glCreateShader(GL_FRAGMENT_SHADER);
			else
				sid = glCreateShader(GL_VERTEX_SHADER);

			glShaderBinary(1, &sid, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, vs_buf.data(), vs_buf.size());
			glSpecializeShaderARB(sid, "main", 0, 0, 0);

			// Check compilation status
			GLint compileStatus;
			glGetShaderiv(sid, GL_COMPILE_STATUS, &compileStatus);
			if (compileStatus == GL_FALSE) {
				// Handle compilation error
				GLint logLength;
				glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &logLength);
				std::vector<char> errorLog(logLength);
				glGetShaderInfoLog(sid, logLength, NULL, errorLog.data());
				// Output error log or handle error appropriately
				std::cerr << "Error compiling shader: " << "it did not compile madafacka" << std::endl;
				// Clean up shader object
				glDeleteShader(sid);
				return;
			}

			mHandle = sid;
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

				
				if (!result) {
					//error
					GLchar InfoLog[256];
					glGetShaderInfoLog(mHandle, sizeof(InfoLog), NULL, InfoLog);
					std::cout << "Error compiling shader: " << InfoLog << std::endl;
				}

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