#include "Model.h"
#include <tinyobjloader.h>
#include <glew.h>
#include <iostream>
#include "glm.hpp"
#include <filesystem>

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   Constructs an empty model class
		*/ //----------------------------------------------------------------------
		Model::Model() noexcept :
			mVAO(NULL), mVBO(NULL), mIBO(NULL), mCount(0), mMaterial(),
			mDiffuseTex(Singleton<ResourceManager>::Instance().GetResource<Texture>("Content/Textures/Brick.png")),
			mNormalTex(Singleton<ResourceManager>::Instance().GetResource<Texture>("Content/Textures/BrickNormal.png")){}
		
		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Clears all verteces from the GPU
		*/ //----------------------------------------------------------------------
		Model::~Model() noexcept {
			Clear();
		}

		// ------------------------------------------------------------------------
		/*! Clear
		*
		*   Clears all verteces from the GPU
		*/ //----------------------------------------------------------------------
		void Model::Clear() noexcept {
			//If there is a valid handle
			if (mVAO > 0) {
				glDeleteVertexArrays(1, &mVAO);
				glDeleteBuffers(1, &mVBO);
				glDeleteBuffers(1, &mIBO);
				mVAO = mVBO = mIBO = -1;
			}
		}

		// ------------------------------------------------------------------------
		/*! Load From File
		*
		*   Loads the a model from file, and uploads it to the GPU
		*/ //----------------------------------------------------------------------
		void Model::LoadFromFile(const std::string_view& inputfile) {
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::vector<tinyobj::real_t> vertices, normals, tangents, bitangents;
			std::vector<int> indexes;
			std::string warn, err;

			std::string directory;
			const size_t last_slash_idx = inputfile.rfind('/');
			if (std::string::npos != last_slash_idx)
			{
				directory = inputfile.substr(0, last_slash_idx);
			}

			std::string_view file = inputfile.substr(inputfile.find_last_of("/\\") + 1);

			auto oldpath = std::filesystem::current_path(); //getting path
			std::filesystem::current_path(directory); //setting path

			//If we had any issue loading the class
			if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file.data()) || !warn.empty() || !err.empty())
				throw ModelException(("Error: " + err + ", warning; " + warn).c_str());

			std::filesystem::current_path(oldpath);
			Clear();

			if (materials.size()) {
				if (!materials[0].diffuse_texname.empty()) {
					mDiffuseTex = Singleton<ResourceManager>::Instance().GetResource<Texture>((directory + '/' + materials[0].diffuse_texname).c_str());
				}

				if (!materials[0].bump_texname.empty()) {
					mNormalTex = Singleton<ResourceManager>::Instance().GetResource<Texture>((directory + '/' + materials[0].bump_texname).c_str());
				}
			}

			// Loop over shapes
			for (size_t s = 0; s < shapes.size(); s++) {
				// Loop over faces(polygon)
				size_t index_offset = 0;
				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
					const size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

					glm::vec3 tangent, bitangent;

					if (fv == 3) {
						tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + 0];

						glm::vec3 pos1 = { attrib.vertices[3 * size_t(idx.vertex_index) + 0] ,
							attrib.vertices[3 * size_t(idx.vertex_index) + 1] , attrib.vertices[3 * size_t(idx.vertex_index) + 2] };

						glm::vec2 uv1 = {
							attrib.texcoords[2 * size_t(idx.texcoord_index) + 0],
							attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]
						};

						idx = shapes[s].mesh.indices[index_offset + 1];

						glm::vec3 pos2 = { attrib.vertices[3 * size_t(idx.vertex_index) + 0] ,
							attrib.vertices[3 * size_t(idx.vertex_index) + 1] , attrib.vertices[3 * size_t(idx.vertex_index) + 2] };

						glm::vec2 uv2 = {
							attrib.texcoords[2 * size_t(idx.texcoord_index) + 0],
							attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]
						};

						idx = shapes[s].mesh.indices[index_offset + 2];

						glm::vec3 pos3 = { attrib.vertices[3 * size_t(idx.vertex_index) + 0] ,
							attrib.vertices[3 * size_t(idx.vertex_index) + 1] , attrib.vertices[3 * size_t(idx.vertex_index) + 2] };

						glm::vec2 uv3 = {
							attrib.texcoords[2 * size_t(idx.texcoord_index) + 0],
							attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]
						};

						// Find T and B
						const glm::vec3 v10 = pos2 - pos1;
						const glm::vec2 uv10 = uv2 - uv1;
						const glm::vec3 v20 = pos3 - pos1;
						const glm::vec2 uv20 = uv2 - uv1;
						const float div = (uv10.y * uv20.x - uv20.y * uv10.x);
						const float mul = div ? 1.0f / div : 0;
						const glm::vec3 t = (uv10.y * v20 - uv20.y * v10) * mul;
						const glm::vec3 b = (uv20.x * v10 - uv10.x * v20) * mul;

						tangent = t;
						bitangent = b;
					}
					else {
						tangent = { 0, 0, 0 };
						bitangent = { 0, 0, 0 };
					}

					// Loop over vertices in the face.
					for (size_t v = 0; v < fv; v++) {
						// access to vertex
						tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

						vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 0]);
						vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 1]);
						vertices.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 2]);

						// Check if `normal_index` is zero or positive. negative = no normal data
						if (idx.normal_index >= 0) {
							vertices.push_back(attrib.normals[3 * size_t(idx.normal_index) + 0]);
							vertices.push_back(attrib.normals[3 * size_t(idx.normal_index) + 1]);
							vertices.push_back(attrib.normals[3 * size_t(idx.normal_index) + 2]);
						}

						// Check if `texcoord_index` is zero or positive. negative = no texcoord data
						if (idx.texcoord_index >= 0) {
							vertices.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]);
							vertices.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]);
						}

						glm::vec3 tnormal = {
							attrib.normals[3 * size_t(idx.normal_index) + 0],
							attrib.normals[3 * size_t(idx.normal_index) + 1],
							attrib.normals[3 * size_t(idx.normal_index) + 2]
						};
						glm::vec3 ttangent = tangent;
						glm::vec3 tbitangent = bitangent;

						GramSchmidt(tnormal, ttangent, tbitangent);

						vertices.push_back(ttangent.x);
						vertices.push_back(ttangent.y);
						vertices.push_back(ttangent.z);
						vertices.push_back(tbitangent.x);
						vertices.push_back(tbitangent.y);
						vertices.push_back(tbitangent.z);
					}
					index_offset += fv;

					if (shapes[s].mesh.material_ids[f] != -1) {
						// per-face material
						/*mMaterial.SetAmbient(glm::vec4(materials[shapes[s].mesh.material_ids[f]].ambient[0],
							materials[shapes[s].mesh.material_ids[f]].ambient[1],
							materials[shapes[s].mesh.material_ids[f]].ambient[2], 1.f));
						mMaterial.SetDiffuse(glm::vec4(materials[shapes[s].mesh.material_ids[f]].diffuse[0],
							materials[shapes[s].mesh.material_ids[f]].diffuse[1],
							materials[shapes[s].mesh.material_ids[f]].diffuse[2], 1.f));
						mMaterial.SetEmissive(glm::vec4(materials[shapes[s].mesh.material_ids[f]].emission[0],
							materials[shapes[s].mesh.material_ids[f]].emission[1],
							materials[shapes[s].mesh.material_ids[f]].emission[2], 1.f));
						mMaterial.SetSpecular(glm::vec4(materials[shapes[s].mesh.material_ids[f]].specular[0],
							materials[shapes[s].mesh.material_ids[f]].specular[1],
							materials[shapes[s].mesh.material_ids[f]].specular[2], 1.f));
						mMaterial.SetShininess(materials[shapes[s].mesh.material_ids[f]].shininess);*/
					}
					else {
						mMaterial.SetAmbient(glm::vec4(1.f, 1.f, 1.f, 1.f));
						mMaterial.SetDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f));
						mMaterial.SetEmissive(glm::vec4(1.f, 1.f, 1.f, 1.f));
						mMaterial.SetSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f));
						mMaterial.SetShininess(1.f);
					}
				}
			}

			UploadToGPU(vertices, indexes);
		}

		void Model::SetShaderUniforms(const Core::Graphics::ShaderProgram& s) const {
			float _shininess = mMaterial.GetShininess();
			glm::vec4 buffer[] = {
				mMaterial.GetEmissive(),
				mMaterial.GetAmbient(),
				mMaterial.GetDiffuse(),
				mMaterial.GetSpecular(),
			};

			//s.SetShaderUniform("uMaterial.emission", buffer);
			//s.SetShaderUniform("uMaterial.ambient", buffer + 1);
			//s.SetShaderUniform("uMaterial.diffuse", buffer + 2);
			//s.SetShaderUniform("uMaterial.specular", buffer + 3);
			//s.SetShaderUniform("uMaterial.shininess", &_shininess);
		}

		void Model::UploadToGPU(std::vector<float>& vertices, std::vector<int>& indexes) {
			glGenVertexArrays(1, &mVAO);
			glGenBuffers(1, &mVBO);
			glGenBuffers(1, &mIBO);

			glBindVertexArray(mVAO);

			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(tinyobj::real_t), vertices.data(), GL_STATIC_DRAW);
			mCount = vertices.size();

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(tinyobj::index_t), indexes.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, ModelVertex::pos));
			glEnableVertexAttribArray(0);

			// Normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, ModelVertex::normal));

			// UVs
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, ModelVertex::uv));

			// UVs
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, ModelVertex::tangent));

			// UVs
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, ModelVertex::bitangent));

			glBindVertexArray(0);
		}

		void Model::GramSchmidt(glm::vec3& n, glm::vec3& t, glm::vec3& b) {
			n = normalize(n);
			// Gram-Schmidt orthogonalization
			glm::vec3 orthoT = t - dot(t, n) * n;

			// Set tangent to (1,0,0) when lenght is 0
			t = glm::length(orthoT) > 0.0f ?
				glm::normalize(orthoT) :
				glm::vec3(1.0f, 0.0f, 0.0f);

			// Compute the new perpendicular bitangent
			glm::vec3 orthoB = cross(n, t);

			b = glm::dot(orthoB, b) > 0.0f ? glm::normalize(orthoB) : -glm::normalize(orthoB);
		}
	}
}