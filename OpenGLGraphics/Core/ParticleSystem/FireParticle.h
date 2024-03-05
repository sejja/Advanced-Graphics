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
			double fireHeight;
			double radius;
			const unsigned int nParticles = 5000;
		};
	}
}

#endif // !_SET_
