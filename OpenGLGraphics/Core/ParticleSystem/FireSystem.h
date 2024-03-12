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

			void InitParticles() override ;
			glm::vec3 GetAbsolutePos(glm::vec3 relativePoint);
			
			float gap = 0.9f;

		private:

			float radiusA = 20.0f;
			float radiusB = 5.0f;
			float radiusC = 15.0f;

			bool IsElipsoid(float x, float y, float z);
			void AddNewParticle(float x, float y, float z);
		};
	}
}

#endif // !_SET_
