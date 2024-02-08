//
//	Model.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 10/04/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _MODEL__H_
#define _MODEL__H_

#include "CommonDefines.h"
#include <string>
#include <glew.h>
#include <vector>
#include <tinyobjloader.h>
#include "Material.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace Core {
	namespace Graphics {
		class Model {
		public:
#pragma region //Declarations
			struct ModelVertex {
				tinyobj::real_t pos[3];
				tinyobj::real_t normal[3];
				tinyobj::real_t uv[2];
				tinyobj::real_t tangent[3];
				tinyobj::real_t bitangent[3];
			};

			CLASS_EXCEPTION(Model)
#pragma endregion

#pragma region //Constructor & Destructors
			Model() noexcept;
			~Model() noexcept;
#pragma endregion

#pragma region //Functions
			void Clear() noexcept;
			void LoadFromFile(const std::string_view& inputfile);
			DONTDISCARD GLuint inline GetHandle() const noexcept;
			DONTDISCARD size_t inline GetVertexCount() const noexcept;
			void SetShaderUniforms(const ShaderProgram& s) const;

		private:
			void UploadToGPU(std::vector<float>& vertices, std::vector<int>& indexes);
			void GramSchmidt(glm::vec3& n, glm::vec3& t, glm::vec3& b);
#pragma endregion

#pragma region //Variables
			std::size_t mCount;
			GLuint mVAO, mVBO, mIBO;
			::Graphics::Material mMaterial;
			Asset<Texture> mDiffuseTex;
			Asset<Texture> mNormalTex;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Handle
		*
		*   Returns the Vertex Array Object assigned by OpenGL
		*/ //----------------------------------------------------------------------
		GLuint Model::GetHandle() const noexcept {
			return mVAO;
		}

		// ------------------------------------------------------------------------
		/*! Get Vertex Count
		*
		*   Returns how many vertex does the model have
		*/ //----------------------------------------------------------------------
		std::size_t Model::GetVertexCount() const noexcept {
			return mCount;
		}
	}
}


#endif