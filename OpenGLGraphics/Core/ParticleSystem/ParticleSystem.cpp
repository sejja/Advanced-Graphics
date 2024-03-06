#include "ParticleSystem.h"
#include <iostream>
namespace Core
{
	namespace Particles
	{
		ParticleSystem::ParticleSystem(const std::weak_ptr<Object>& parent) : Renderable(parent){
            //initTestParticles(); 
            //init();
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
        void ParticleSystem::initTestParticles()
        {
            for (float x = (center.x - width / 2); x < (center.x + width / 2); x += (width / nParticlesTest))
            {
                for (float z = (center.z - width / 2); z < (center.z + width / 2); z += (width / nParticlesTest))
                {
                    for (float y = center.y; y < (center.y + height); y += (height / nParticlesTest))
                    {
                        Particle particle = {
                            glm::vec3(x, y, z),//pos
                            glm::vec3(10.f,10.f,10.f)
                        };
                        this->particles.push_back(particle);

                    }
                }
            }
        }
        void ParticleSystem::init()
        {
            glGenVertexArrays(1, &VAO);

            //Gen an instanced vertex buffer
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, this->particles.size() * sizeof(Particle), this->particles.data(), GL_DYNAMIC_DRAW);

            glBindVertexArray(VAO);

            //Configure shift pos
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, pos));
            glVertexAttribDivisor(0, 1);

            // Configure shift velocity
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
            glVertexAttribDivisor(1, 1);

            glBindVertexArray(0);

            shaderProgram = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Particle.shader");
        }

	}
}
