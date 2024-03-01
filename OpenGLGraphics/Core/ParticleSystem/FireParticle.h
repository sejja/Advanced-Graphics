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

			FireSystem();
			~FireSystem();
		};
	}
}

#endif // !_SET_
