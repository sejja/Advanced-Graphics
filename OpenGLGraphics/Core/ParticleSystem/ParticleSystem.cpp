#include "ParticleSystem.h"
#include <iostream>
namespace Core
{
	namespace Particles
	{
		ParticleSystem::ParticleSystem(const std::weak_ptr<Object>& parent) : Renderable(parent){

            for (float x = (center.x - width/2); x < (center.x + width/2); x+=(width / nParticlesTest) )
            {
                for (float z = (center.z - width / 2); z < (center.z + width / 2); z += (width / nParticlesTest))
                {
                    for(float y = center.y; y < (center.y + height); y += (height/nParticlesTest) )
                    {
                        Particle particle = {
                            glm::vec3(x, y, z),//pos
                            particleSize,
                            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
                            100
                        };

                        this->particles.push_back(particle);
                    }
                }
            }

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, this->particles.size() * sizeof(Particle), this->particles.data(), GL_STATIC_DRAW);

            // Posición
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, pos));

            glBindVertexArray(0);

            shaderProgram = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Particle.shader");

		}

		ParticleSystem::~ParticleSystem()
		{
		}

        void ParticleSystem::SetCameraReference(Core::Primitives::Camera* camera)
        {
            this->camera = camera;
        }

        Core::Primitives::Camera* ParticleSystem::GetCameraReference()
        {
            return camera;
        }

        void ParticleSystem::Update()
		{
			std::for_each(particles.begin(), particles.end(), [this](Core::Particles::Particle& particle) {
				ParticleFunction(&particle);
			});
		}

		int ParticleSystem::ParticleFunction(Particle* p_particle)
		{
            //std::cout << "Unimplemented methdod ParticleFunction of Default Particle System \n";
			return -1;
		}
	}
}
