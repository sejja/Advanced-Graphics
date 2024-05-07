#include <iostream>
#include "FireSystem.h"
#include "Graphics/Primitives/Renderables.h"

namespace Core {
	namespace Particles {

		/// <summary>
		/// Default builder, default center = (0.0f, 0.0f, 75.0f)
		/// </summary>
		/// <param name="parent"></param>
		FireSystem::FireSystem(const std::weak_ptr<Object>& parent): ParticleSystem(parent) {
            InitParticles();
            Init();
            shaderProgram = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/FireParticle.shader");
            particleSize = 4.0f; 
            baseColor = NormalizeRGBA(252, 186, 3, 255);
            acceleration = glm::vec3(0.0f, 0.5f, 0.0f);
            SetSystemCenter(glm::vec3(0.0f, 0.0f, 75.0f)); 
            this->name = "fuego";
            this->id = "fuego";
		}

        /*The center of the system is in 0,0,0 by default*/
        FireSystem::FireSystem(const std::weak_ptr<Object>& parent, float radiusA, float radiusB, float radiusC, float gap, float height, float acceleration) : ParticleSystem(parent)
        {
            this->radiusA = radiusA;
            this->radiusB = radiusB;
            this->radiusC = radiusC;
            this->gap = gap;
            this->height = height;
            this->acceleration = glm::vec3(0.0f, acceleration, 0.0f);

            InitParticles();
            Init();

            shaderProgram = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/FireParticle.shader");

            particleSize = 4.0f;
            baseColor = NormalizeRGBA(252, 186, 3, 255);
        }

		FireSystem::~FireSystem()
		{
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteShader(shaderProgram->Get()->GetHandle());
		}

        void FireSystem::ChangeFireSize(float radiusA, float radiusB, float radiusC, float gap, float height)
        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);

            this->radiusA = radiusA;
            this->radiusB = radiusB;
            this->radiusC = radiusC;
            this->gap = gap;
            this->height = height;

            this->particles.clear();
            InitParticles();
            Init();
        }



        /// <summary>
        ///  There is only getters for the radius and not setters 
        /// </summary>
        /// <returns> (x = radiusA, y = radiusB, z = radiusC) </returns>
        glm::vec3 FireSystem::GetRadiusVector()
        {
            return glm::vec3(this->radiusA, this->radiusB, this->radiusC);
        }

        float FireSystem::GetFireGap()
        {
			return this->gap;
		}
        float FireSystem::GetFireHeight()
        {
            return this->height;
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