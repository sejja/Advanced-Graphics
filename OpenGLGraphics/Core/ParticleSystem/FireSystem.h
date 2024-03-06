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
			FireSystem(const std::weak_ptr<Object>& parent);
			~FireSystem();
		public:
			double fireHeight;
			double radius;
		protected:
			void init() override;
		private:
		};
	}
}

#endif // !_SET_
