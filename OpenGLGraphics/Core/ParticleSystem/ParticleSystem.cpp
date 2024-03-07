#include "ParticleSystem.h"
#include <iostream>
namespace Core
{
	namespace Particles
	{
        /*It does not initiate particles, only the VBO, BAO and ShaderProgram*/
		ParticleSystem::ParticleSystem(const std::weak_ptr<Object>& parent) : Renderable(parent){
            //initTestParticles(); 
            std::cout << "ERROR: DEFAULT PARTICLESYSTEM HAS NO PARTICLES \n";
            Init();
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

        void ParticleSystem::SetSystemCenter(glm::vec3 newCenter)
        {
            center = newCenter;
        }

        glm::vec3 ParticleSystem::GetSystemCenter()
        {
            return this->center;
        }

        /*It does nothing by default*/
        void ParticleSystem::Update()
		{
            delta++;
            return;
			std::for_each(particles.begin(), particles.end(), [this](Core::Particles::Particle& particle) {
				ParticleFunction(&particle);
			});
		}

        /*It does nothing by default, in case of activate it on Update() it has a CPU cost*/
		int ParticleSystem::ParticleFunction(Particle* p_particle)
		{
            //std::cout << "Unimplemented methdod ParticleFunction of Default Particle System \n";
			return -1;
		}

        void ParticleSystem::Init()
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

            glBindVertexArray(0);

            shaderProgram = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Particle.shader");
        }

        /*Initiate the vector or particles*/
        void ParticleSystem::InitParticles()
        {
            std::cout << "ERROR: UNIMPLEMENTED FUNCTION INITPARTICLES\n";
        }

        glm::vec4 ParticleSystem::NormalizeRGBA(int R, int G, int B, int A)
        {
            float red = (R / 255.0f);
            float green = (G / 255.0f);
            float blue = (B / 255.0f);
            float alpha = (A / 255.0f);
            return glm::vec4( red, green, blue, alpha);
        }

	}
}
