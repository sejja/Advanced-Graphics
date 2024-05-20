#include <iostream>
#include <memory>
#include "SmokeSystem.h"

namespace Core
{
	namespace Particles
	{
		SmokeSystem::SmokeSystem(const std::weak_ptr<Object>& parent) : FireSystem(parent)
		{

		}

		SmokeSystem::SmokeSystem(const std::weak_ptr<Object>& parent, float radiusA, float radiusB, float radiusC, float gap, float height, float acceleration)
			: FireSystem(parent, radiusA, radiusB, radiusC, gap, height, acceleration)
		{
			baseColor = NormalizeRGBA(225, 225, 225, 225);
			this->name = "humo";
			this->id = "humo";
		}

		SmokeSystem::~SmokeSystem()
		{

		}
	}
}