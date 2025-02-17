//
//	ShaderProgram.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2024. All Rights reserved
//

#ifndef _SHADER_PROGRAM__H_
#define _SHADER_PROGRAM__H_

#include <string>
#include <glew.h>
#include "glm.hpp"
#include "Shader.h"
#include "Color.h"
#include "CommonDefines.h"
#include "Core/Assets/ResourceManager.h"

namespace Core {
	namespace Graphics {
		class ShaderProgram {
		#pragma region //Declarations
		public:
			CLASS_EXCEPTION(ShaderProgram)
		#pragma endregion

		#pragma region //Constructors
			ShaderProgram() noexcept;
			ShaderProgram(const Core::Assets::AssetReference<Shader>& vertexShader, const Core::Assets::AssetReference<Shader>& fragmentShader);
			ShaderProgram(const Core::Assets::AssetReference<Shader>& vertexShader, const Core::Assets::AssetReference<Shader>& fragmentShader,
				const Core::Assets::AssetReference<Shader>& geometryShader);
			~ShaderProgram() noexcept;
		#pragma endregion

		#pragma region //Methods
			void inline Bind() const noexcept;
			void inline SetShaderUniform(const std::string_view& name, const int value);
			void inline SetShaderUniform(const std::string_view& name, const bool value);
			void inline SetShaderUniform(const std::string_view& name, int* value, const int count = 1);
			void inline SetShaderUniform(const std::string_view& name, float* value, const int count = 1);
			void inline SetShaderUniform(const std::string_view& name, glm::mat4* value, const int count = 1);
			void inline SetShaderUniformMatrix4d(const std::string_view& name, glm::mat4* value, const int count = 1);
			void inline SetShaderUniform(const std::string_view& name, glm::mat3* value, const int count = 1);
			void inline SetShaderUniform(const std::string_view& name, glm::vec2* value, const int count = 1);
			void inline SetShaderUniform(const std::string_view& name, glm::ivec2* value, const int count = 1);
			void inline SetShaderUniform(const std::string_view& name, glm::vec3* value, const int count = 1);
			void inline SetShaderUniform(const std::string_view& name, glm::vec4* value, const int count = 1);
			void inline SetShaderUniform(const std::string_view& name, ::Graphics::Primitives::Color* value, const int count = 1);
			void inline SetShaderUniform(const std::string_view& name, float value);
			void ReloadShader(Core::Assets::Asset<Shader>& vertexShader, Core::Assets::Asset<Shader>& fragmentShader);
			GLuint GetHandle();
		private:
			GLuint getUniformLocation(const std::string_view& id);
		#pragma endregion

		#pragma region //Members
			std::unordered_map<std::string_view, GLuint> mUniformLocations;
			unsigned int mHandle;
		#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of an integer
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, const int value) {
			glUniform1i(getUniformLocation(name), value);
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of a boolean
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, const bool value) {
			glUniform1i(getUniformLocation(name), value);
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of integers
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, int* value, const int count) {
			glUniform1iv(getUniformLocation(name), count, value);
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of floats
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, float* value, const int count) {
			glUniform1fv(getUniformLocation(name), count, value);
		}
		void ShaderProgram::SetShaderUniform(const std::string_view& name, float value) {
			glUniform1f(getUniformLocation(name), value);
		}
		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Matrices 4x4
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::mat4* value, const int count) {
			glUniformMatrix4fv(getUniformLocation(name), count, GL_FALSE, reinterpret_cast<float*>(value));
		}
		void ShaderProgram::SetShaderUniformMatrix4d(const std::string_view& name, glm::mat4* value, const int count) {
			glUniformMatrix4fv(getUniformLocation(name), count, GL_FALSE, reinterpret_cast<float*>(value));
		}
		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of matrices 3x3
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::mat3* value, const int count) {
			glUniformMatrix4fv(getUniformLocation(name), count, GL_FALSE, reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Vectors of size 2
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::vec2* value, const int count) {
			glUniform2fv(getUniformLocation(name), count, reinterpret_cast<float*>(value));
		}

		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::ivec2* value, const int count) {
			glUniform2iv(getUniformLocation(name), count, reinterpret_cast<int*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Vectors of size 3
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::vec3* value, const int count) {
			glUniform3fv(getUniformLocation(name), count, reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Vectors of size 4
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, glm::vec4* value, const int count) {
			glUniform4fv(getUniformLocation(name), count, reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of a Color
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const std::string_view& name, ::Graphics::Primitives::Color* value, const int count) {
			glUniform4fv(getUniformLocation(name), count, reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Get Handle
		*
		*   Returns the OpenGL handle
		*/ // --------------------------------------------------------------------
		inline GLuint ShaderProgram::GetHandle() {
			return mHandle;
		}

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Binds the Shader Program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::Bind() const noexcept {
			glUseProgram(mHandle);
		}
	}
}

#endif
