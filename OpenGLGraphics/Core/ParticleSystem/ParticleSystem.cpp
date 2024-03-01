#include "Core/ParticleSystem/ParticleSystem.h"
#include <iostream>
namespace Core
{
	namespace Particles
	{
		ParticleSystem::ParticleSystem(const std::weak_ptr<Object>& parent) : Renderable(parent){
            Particle particles[4] = {
                // Partícula 1
                {
                    glm::vec3(0.0f, 1.0f, 0.0f), // pos
                    glm::vec2(1.0f, 1.0f),       // size
                    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // color (rojo)
                    100                          // lifeTime
                },
                // Partícula 2
                {
                    glm::vec3(1.0f, 0.0f, 0.0f), // pos
                    glm::vec2(0.5f, 0.5f),       // size
                    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // color (verde)
                    200                          // lifeTime
                },
                // Partícula 3
                {
                    glm::vec3(-1.0f, 0.0f, 0.0f),// pos
                    glm::vec2(0.75f, 0.75f),     // size
                    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // color (azul)
                    150                          // lifeTime
                },
                // Partícula 4
                {
                    glm::vec3(0.0f, -1.0f, 0.0f),// pos
                    glm::vec2(1.2f, 1.2f),      // size
                    glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // color (amarillo)
                    250                          // lifeTime
                }
            };

            for (int i = 0; i < sizeof(particles) / sizeof(Particle); i++) {
                this->particles.push_back(particles[i]);
            }
		}

		ParticleSystem::~ParticleSystem()
		{
		}

		void ParticleSystem::Update()
		{
			std::for_each(particles.begin(), particles.end(), [this](Core::Particles::Particle& particle) {
				ParticleFunction(&particle);
			});
		}

		int ParticleSystem::ParticleFunction(Particle* p_particle)
		{
            std::cout << "Unimplemented methdod ParticleFunction of Default Particle System \n";
			return -1;
		}
	}
}
