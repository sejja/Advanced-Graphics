//
//	ShaderProgram.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2024 . All Rights reserved
//

#include <glew.h>
#include "Core/Assets/ResourceManager.h"
#include "ShaderProgram.h"
#include <iostream>

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a Shader Program Class
		*/ // --------------------------------------------------------------------
		ShaderProgram::ShaderProgram() noexcept :
			mHandle(glCreateProgram()) {}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Deallocates a program from the GPU
		*/ // --------------------------------------------------------------------
		ShaderProgram::~ShaderProgram() noexcept {
			glDeleteProgram(mHandle);
		}

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Shader Program class given a Vertex and Fragment Shader
		*/ // ---------------------------------------------------------------------
		ShaderProgram::ShaderProgram(const Core::Assets::AssetReference<Shader>& vertexShader, const Core::Assets::AssetReference<Shader>& fragmentShader)
			: ShaderProgram() {
			//If both of the assets contain valid data
			if (!vertexShader.expired() && !fragmentShader.expired()) {
				glAttachShader(mHandle, vertexShader.lock()->Get()->GetGLHandle());
				glAttachShader(mHandle, fragmentShader.lock()->Get()->GetGLHandle());
				glLinkProgram(mHandle);

				GLint status;
				glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
				GLchar ErrorLog[1024] = { 0 };
				if (status == GL_FALSE) {
					glGetProgramInfoLog(mHandle, sizeof(ErrorLog), NULL, ErrorLog);
					fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
				}

				if (status == GL_FALSE) throw ShaderProgramException("Shader Program Failed to Link");
			}
		}
		void ShaderProgram::ReloadShader(Core::Assets::Asset<Shader>& vertexShader, Core::Assets::Asset<Shader>& fragmentShader) {
			if (mHandle) {
				glDeleteProgram(mHandle);
			}

			mHandle = glCreateProgram();
			glAttachShader(mHandle, vertexShader->Get()->GetGLHandle());
			glAttachShader(mHandle, fragmentShader->Get()->GetGLHandle());
			glLinkProgram(mHandle);

			GLint status;
			glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

			if (status == GL_FALSE)
				throw ShaderProgramException("Shader Program Failed to Link");

		}
		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Shader Program class given a Vertex, Geometry & Fragment Shader
		*/ // ---------------------------------------------------------------------
		ShaderProgram::ShaderProgram(const Core::Assets::AssetReference<Shader>& vertexShader, const Core::Assets::AssetReference<Shader>& fragmentShader, const Core::Assets::AssetReference<Shader>& geometryShader)
			: ShaderProgram() {
			//If both of the assets contain valid data
			if (!vertexShader.expired() && !fragmentShader.expired()) {
				glAttachShader(mHandle, vertexShader.lock()->Get()->GetGLHandle());
				glAttachShader(mHandle, fragmentShader.lock()->Get()->GetGLHandle());
				glAttachShader(mHandle, geometryShader.lock()->Get()->GetGLHandle());
				glLinkProgram(mHandle);

				GLint status;
				glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

				if (status == GL_FALSE) throw ShaderProgramException("Shader Program Failed to Link");
			}
		}

		// ------------------------------------------------------------------------
		/*! Get Uniform Location
		*
		*  Get the location of an Uniform within a shader
		*/ // --------------------------------------------------------------------
		GLuint ShaderProgram::getUniformLocation(const std::string_view& id) {
			const std::unordered_map<std::string_view, GLuint>::local_iterator idx = mUniformLocations.find(id);

		#if _DEBUG
			int i = idx == mUniformLocations.end() ? (mUniformLocations[id] = glGetUniformLocation(mHandle, id.data())) : idx->second;

			if (i == -1) {
				//std::cout << "There is no such uniform in the shader" << std::endl;
				throw ShaderProgramException("There is no such uniform in the shader");
			}

			return i;
		#else
			return idx == mUniformLocations.end() ? (mUniformLocations[id] = glGetUniformLocation(mHandle, id.data())) : idx->second;
		#endif
		}
	}
}