#ifndef _PARTICLESYSTEM_
#define _PARTICLESYSTEM_

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Core/ECSystem/Object.h"
#include "Core/ECSystem/Component.h"
#include "Graphics/Primitives/Renderables.h"
#include "Core/ParticleSystem/ParticleManager.h"
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

				void Update() override;

				virtual void Render() const override {
					shaderProgram->Get()->Bind();

					std::shared_ptr<Object> sharedParent = mParent.lock();
					std::shared_ptr<Core::Particles::ParticleManager> particleManager = std::dynamic_pointer_cast<Core::Particles::ParticleManager>(sharedParent);
					Core::Primitives::Camera* camera = particleManager.get()->getCameraReference();

					std::for_each(particles.begin(), particles.end(), [this, camera](Particle particle)
					{
							std::cout << "Render Particle...\n";
							//-------------------------------------------------------
							glm::mat4 model = glm::mat4(1.0f);
							model = glm::translate(model, particle.pos); // Mueve el objeto a (x, y, z) en el espacio mundial
							model = glm::scale(model, particle.size); // Escala el objeto

							shaderProgram->Get()->SetShaderUniform("model", &model);
							shaderProgram->Get()->SetShaderUniform("view", &(camera->GetViewMatrix()));
							shaderProgram->Get()->SetShaderUniform("projection", &projection);
							//-------------------------------------------------------
							glBindVertexArray(0);
					});
				};

			protected:
				unsigned int VBO, VAO;
				Asset<Core::Graphics::ShaderProgram> shaderProgram;
				std::vector<Particle> particles;
				glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);

				int ParticleFunction(Particle *p_particle);

		};
	}
}

#endif // !_PARTICLESYSTEM_
