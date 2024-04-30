#include <iostream>
#include "InstancedMesh.h"
#include "gtc/matrix_transform.hpp"

namespace Graphics {
	namespace Architecture {
		namespace InstancedRendering {
			InstancedMesh::InstancedMesh()
			{
				//this->asociatedMesh = std::make_shared<Graphics::Primitives::Mesh>(asociatedMesh);

				glGenBuffers(1, &ubo);

				GLuint transformBlockIndex = glGetUniformBlockIndex(shaderProgram->GetHandle(), "TransformBlock");
				glUniformBlockBinding(shaderProgram->GetHandle(), transformBlockIndex, 1);
			}
			InstancedMesh::~InstancedMesh()
			{

			}

			void InstancedMesh::drawInstanced(std::shared_ptr<std::vector<std::shared_ptr<Core::Object>>> instancedObject) const
			{
				std::cout << "UNIMPLEMENTED FUNCTION drawInstanced IN InstancedMesh: " << this << "\n";

				this->asociatedMesh->bindTextures();

				// draw mesh
				glBindVertexArray(this->asociatedMesh->getVao());
				glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(this->asociatedMesh->getCount()), GL_UNSIGNED_INT, 0, static_cast<int>(instancedObject->size()));

				glBindBuffer(GL_UNIFORM_BUFFER, ubo);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4) * transforms.size(), transforms.data());

				// always good practice to set everything back to defaults once configured.

				glBindVertexArray(0);
				glActiveTexture(GL_TEXTURE0);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}

			void InstancedMesh::updateTransforms(std::shared_ptr<std::vector<std::shared_ptr<Core::Object>>> instancedObjects)
			{
				this->getTransforms_ptr()->clear();
				std::for_each(instancedObjects.get()->begin(), instancedObjects.get()->end(), [this](std::shared_ptr<Core::Object > parent) {
					glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->GetPosition()) *
						glm::rotate(glm::mat4(1.0f), parent->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f)) *
						glm::rotate(glm::mat4(1.0f), parent->GetRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)) *
						glm::rotate(glm::mat4(1.0f), parent->GetRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)) *
						glm::scale(glm::mat4(1.0f), parent->GetScale());

					//std::cout << "Instanced Rendering: " << parent.get() << "\n";
					this->getTransforms_ptr()->push_back(matrix);
				});
			}

			void InstancedMesh::setAssociatedMesh(Graphics::Primitives::Mesh* mesh)
			{
				this->asociatedMesh = std::make_shared<Graphics::Primitives::Mesh>(*mesh);
			}

			std::vector<glm::mat4>* InstancedMesh::getTransforms_ptr()
			{
				return &(this->transforms);
			}
		}
	}
}