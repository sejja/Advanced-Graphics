#include "FireSystem.h"
#pragma once

#ifndef _SMOKESYSTEM_
#define _SMOKESYSTEM_

namespace Core
{
	namespace Particles
	{
		class SmokeSystem : public FireSystem
		{
			SmokeSystem(const std::weak_ptr<Object>& parent);
			SmokeSystem(const std::weak_ptr<Object>& parent, float radiusA, float radiusB, float radiusC, float gap, float height, float acceleration);
			~SmokeSystem();

		};
	}
}

#endif // !_SMOKESYSTEM_
