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
			glm::vec3 size;
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
					glm::mat4 uProjection = projection;

					std::for_each(particles.begin(), particles.end(), [this, view, uProjection](Particle particle) 
					{

						//std::cout << "--->Render  particle \n";

						glm::mat4 model = glm::mat4(1.0f);
						model = glm::translate(model, particle.pos); // Mueve el objeto a (x, y, z) en el espacio mundial
						model = glm::scale(model, particle.size); // Escala el objeto
						shaderProgram->Get()->SetShaderUniform("model", &model);
						shaderProgram->Get()->SetShaderUniform("view", &view);
						shaderProgram->Get()->SetShaderUniform("projection", &uProjection);

						// Vincula el VAO
						glBindVertexArray(VAO);

						// Dibuja las partículas
						glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particles.size()));

						glBindVertexArray(0);
					});
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
