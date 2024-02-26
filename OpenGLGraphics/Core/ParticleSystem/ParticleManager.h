#ifndef _PARTICLEMANAGER_
#define _PARTICLEMANAGER_

#include "glm.hpp"
#include "Core/ECSystem/Object.h"

namespace Core
{
	namespace Particles 
	{
		class ParticleMangager : public Core::Object
		{
			void Update();
		};
	}
}
#endif 

