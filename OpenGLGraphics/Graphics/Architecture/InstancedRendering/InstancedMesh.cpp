#include <iostream>
#include "InstancedMesh.h"
#include "gtc/matrix_transform.hpp"

namespace Graphics {
	namespace Architecture {
		namespace InstancedRendering {
			InstancedMesh::InstancedMesh()
			{
				
				//generate the uniform buffer that contains the 
				glGenBuffers(1, &ubo);
				glBindBuffer(GL_UNIFORM_BUFFER, ubo);
				glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
				GLuint transformBlockIndex = glGetUniformBlockIndex(shaderProgram->GetHandle(), "TransformBlock");
				glUniformBlockBinding(shaderProgram->GetHandle(), transformBlockIndex, 1);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);

				this->transforms = new std::vector<glm::mat4>();
			}
			InstancedMesh::~InstancedMesh()
			{
				delete this->transforms;
				glDeleteBuffers(GL_UNIFORM_BUFFER, &ubo);
			}

			void InstancedMesh::drawInstanced(std::shared_ptr<std::vector<std::shared_ptr<Core::Object>>> instancedObject)
			{

			// Bind all needed textures
				this->asociatedMesh->bindTextures();

			// Bind the VAO
				glBindVertexArray(this->asociatedMesh->getVao());

			// Bind the uniform buffer
				glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);
				glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * this->transforms->size(), this->transforms->data(), GL_DYNAMIC_DRAW);

			// Draw call
				glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(this->asociatedMesh->getCount()), GL_UNSIGNED_INT, 0, static_cast<int>(transforms->size()));
			
			// Set to the default for the next render
				glBindVertexArray(0);
				glActiveTexture(GL_TEXTURE0);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}

			void InstancedMesh::updateTransforms(std::shared_ptr<std::vector<std::shared_ptr<Core::Object>>> instancedObjects)
			{
				this->transforms->clear();
				std::for_each(instancedObjects.get()->begin(), instancedObjects.get()->end(), [this](std::shared_ptr<Core::Object > parent) {
					glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->GetPosition()) *
						glm::rotate(glm::mat4(1.0f), parent->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f)) *
						glm::rotate(glm::mat4(1.0f), parent->GetRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)) *
						glm::rotate(glm::mat4(1.0f), parent->GetRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)) *
						glm::scale(glm::mat4(0.01f), parent->GetScale());
					this->transforms->push_back(matrix);
				});
			}

			void InstancedMesh::setAssociatedMesh(Graphics::Primitives::Mesh* mesh)
			{
				this->asociatedMesh = std::make_shared<Graphics::Primitives::Mesh>(*mesh);
			}


			int* InstancedMesh::range(int n) const {
				int* array = new int[n]; // Allocate memory for an array with n+1 elements
				for (int i = 0; i < n; i++) {
					array[i] = i; // Fill the array with numbers from 0 to n
				}
				return array;
			}
		}
	}
}