#include <iostream>
#include "FireSystem.h"
#include "Graphics/Primitives/Renderables.h"

namespace Core {
	namespace Particles {
		FireSystem::FireSystem(const std::weak_ptr<Object>& parent): ParticleSystem(parent) {
            initParticles();
            init();
            particleSize = 1.0f;
            this->center = glm::vec3(0.0f,0.0f,100.0f);
		}
		FireSystem::~FireSystem()
		{
		}
        void FireSystem::initParticles()
        {
            for (float x = (center.x - (radiusA)); x < (center.x + (radiusA)); x += ( gap) )
            {
                for (float z = (center.z - (radiusC)); z < (center.z + (radiusC)); z += ( gap) )
                {
                    for (float y = (center.y - (radiusB)); y < (center.y + (radiusB)); y += ( gap) )
                    {
                        if (isElipsoid(x, y, z))
                        {
                            addNewParticle(x, y, z);
                        }
                    }
                }
            }
        }

        glm::vec3 FireSystem::originVector(glm::vec3 cardinalPoint)
        {
            return cardinalPoint - this->center;
        }

        bool FireSystem::isElipsoid(float x, float y, float z)
        {
            glm::vec3 originPoint = originVector(glm::vec3(x,y,z));
            x = originPoint.x;
            y = originPoint.y;
            z = originPoint.z;

            float limitX = radiusA * sqrt(1 - pow((y / radiusB), 2) - pow((z / radiusC), 2) );
            float limitY = radiusB * sqrt(1 - pow((x / radiusA), 2) - pow((z / radiusC), 2) );
            float limitZ = radiusC * sqrt(1 - pow((x / radiusA), 2) - pow((y / radiusB), 2) );

            bool ret = ((x < limitX) && (y < limitY) && (z < limitZ));

            return ret;
        }

        void FireSystem::addNewParticle(float x, float y, float z)
        {
            Particle particle = {
                glm::vec3(x, y, z),//pos
                glm::vec3(0.f,0.f,0.f) //vel
            };
            this->particles.push_back(particle);
        }

	}
}