//
//	ShaderProgram.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <glew.h>
#include "Core/ResourceManager.h"
#include "ShaderProgram.h"

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
		ShaderProgram::ShaderProgram(const AssetReference<Shader>& vertexShader, const AssetReference<Shader>& fragmentShader)
			: ShaderProgram() {
			//If both of the assets contain valid data
			if (!vertexShader.expired() && !fragmentShader.expired()) {
				glAttachShader(mHandle, vertexShader.lock()->Get()->GetGLHandle());
				glAttachShader(mHandle, fragmentShader.lock()->Get()->GetGLHandle());
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

			if(i == -1) throw ShaderProgramException("There is no such uniform in the shader");

			return i;
		#else
			return idx == mUniformLocations.end() ? (mUniformLocations[id] = glGetUniformLocation(mHandle, id.data())) : idx->second;
		#endif
		}
	}
}