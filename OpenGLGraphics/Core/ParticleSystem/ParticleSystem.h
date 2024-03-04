#ifndef _PARTICLESYSTEM_
#define _PARTICLESYSTEM_

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Core/ECSystem/Object.h"
#include "Core/ECSystem/Component.h"
#include "Graphics/Primitives/Renderables.h"
#include "Graphics/Camera.h"
#include <iostream>

namespace Core
{
	namespace Particles
	{
		struct Particle
		{
			glm::vec3 pos;
			float size;
			glm::vec4 color;
			unsigned int lifeTime = 0;
		};

		class ParticleSystem : public Core::Graphics::Renderable
		{
			public:
				glm::vec3 center;
				glm::vec3 velocity;
				glm::vec3 acceleration;

				ParticleSystem(const std::weak_ptr<Object>& parent);
				~ParticleSystem();
				
				void SetCameraReference(Core::Primitives::Camera* camera);
				Core::Primitives::Camera* GetCameraReference();
				void Update() override;

				virtual void Render() const override {

					shaderProgram->Get()->Bind();

					Core::Primitives::Camera* camReference = camera;
					glm::mat4 view = camReference->GetViewMatrix();
					glm::mat4 uProjection = this->projection;

					glDisable(GL_DEPTH_TEST);
					glDisable(GL_CULL_FACE);
					//glEnable(GL_DEPTH_TEST);

					//glBindBuffer(GL_ARRAY_BUFFER, VBO);

					std::for_each(particles.begin(), particles.end(), [this, &view, &uProjection](Particle particle) 
					{

						//std::cout << "--->Render  particle \n";
						glm::mat4 model = glm::mat4(1.0f);
						model = glm::translate(model, particle.pos); // Mueve el objeto a (x, y, z) en el espacio mundial
						
						shaderProgram->Get()->SetShaderUniformMatrix4d("model", &model);
						shaderProgram->Get()->SetShaderUniformMatrix4d("view", &view);
						shaderProgram->Get()->SetShaderUniformMatrix4d("projection", &uProjection);
						shaderProgram->Get()->SetShaderUniform("pointSize", particle.size);
						shaderProgram->Get()->SetShaderUniform("particleColor", &particle.color);

						// Vincula el VAO
						glBindVertexArray(VAO);

						// Dibuja las partículas
						glDrawArrays(GL_POINTS, 0, 1);

						glBindVertexArray(0);
					});

					glEnable(GL_DEPTH_TEST);
					glEnable(GL_CULL_FACE);
				};

			protected:
				GLuint VAO, VBO;
				Asset<Core::Graphics::ShaderProgram> shaderProgram;
				std::vector<Particle> particles;
				int ParticleFunction(Particle *p_particle);
				Core::Primitives::Camera* camera;
				glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
		};
	}
}

#endif // !_PARTICLESYSTEM_
