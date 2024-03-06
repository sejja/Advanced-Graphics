#include<iostream>
#include"Core/ParticleSystem/ParticleSystem.h"

#ifndef _FIRE_
#define _FIRE_

namespace Core
{
	namespace Particles
	{
		class FireSystem : public ParticleSystem
		{
		public:
			FireSystem(const std::weak_ptr<Object>& parent);
			~FireSystem();
		protected:
			void initParticles() ;
			glm::vec3 originVector(glm::vec3 cardinalPoint);


			float gap = 0.5f;

		private:
			float radiusA = 10.0f;
			float radiusB = 30.0f;
			float radiusC = 10.0f;

			bool isElipsoid(float x, float y, float z);
			void addNewParticle(float x, float y, float z);
		};
	}
}

#endif // !_SET_
