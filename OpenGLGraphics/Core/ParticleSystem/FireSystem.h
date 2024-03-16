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
			FireSystem(const std::weak_ptr<Object>& parent, float radiusA, float radiusB, float radiusC, float gap, float height, float acceleration);
			~FireSystem();
			
			void ChangeFireSize(float radiusA, float radiusB, float radiusC, float gap, float height);
			glm::vec3 GetRadiusVector();

		protected:

			void InitParticles() override ;
			glm::vec3 GetAbsolutePos(glm::vec3 relativePoint);
			
			float gap = 0.7f;

		private:

			float radiusA = 5.0f;
			float radiusB = 2.0f;
			float radiusC = 5.0f;

			bool IsElipsoid(float x, float y, float z);
			void AddNewParticle(float x, float y, float z);
		};
	}
}

#endif // !_SET_
