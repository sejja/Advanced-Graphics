#include <iostream>
#include "FireSystem.h"
#include "Graphics/Primitives/Renderables.h"

namespace Core {
	namespace Particles {
		FireSystem::FireSystem(const std::weak_ptr<Object>& parent): ParticleSystem(parent) {
            InitParticles();
            Init();
            shaderProgram = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/FireParticle.shader");
            particleSize = 2.0f; 
            baseColor = NormalizeRGBA(252, 186, 3, 255);
            acceleration = glm::vec3(0.0f, 0.5f, 0.0f);
            height = radiusB*2;
            SetSystemCenter(glm::vec3(0.0f, -15.0f, 75.0f)); 
		}
		FireSystem::~FireSystem()
		{
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            shaderProgram->Get()->~ShaderProgram();
		}

        /*This function initiate a elipsoid*/
        void FireSystem::InitParticles()
        {
            for (float x = (-(radiusA)); x < (radiusA); x += (gap) )
            {
                for (float z = (-(radiusC)); z < (radiusC); z += (gap) )
                {
                    for (float y = (-(radiusB)); y <  (radiusB) ; y += (gap) )
                    {
                        if (IsElipsoid(x, y, z))
                        {
                            AddNewParticle(RandomFloat(x + gap, x - gap), RandomFloat(y + gap, y - gap), RandomFloat(z + gap, z - gap)); //This is a discrete ~ normal distribution
                        }
                    }
                }
            }
        }

        glm::vec3 FireSystem::GetAbsolutePos(glm::vec3 relativePoint)
        {
            return relativePoint + this->center;
        }

        bool FireSystem::IsElipsoid(float x, float y, float z)
        {
            return (( (x * q_rsqrt(1 - pow((y / radiusB), 2) - pow((z / radiusC), 2))) < radiusA ) &&
                    ( (y * q_rsqrt(1 - pow((x / radiusA), 2) - pow((z / radiusC), 2))) < radiusB ) &&
                    ( (z * q_rsqrt(1 - pow((x / radiusA), 2) - pow((y / radiusB), 2))) < radiusC ));
        }

        void FireSystem::AddNewParticle(float x, float y, float z)
        {
            Particle particle = {
                glm::vec3(x, y, z)//pos
            };
            this->particles.push_back(particle);
        }

	}
}