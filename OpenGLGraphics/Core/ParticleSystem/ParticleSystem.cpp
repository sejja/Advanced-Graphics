#include "Core/ParticleSystem/ParticleSystem.h"

namespace Core
{
	namespace Particles
	{
		ParticleSystem::ParticleSystem(const std::weak_ptr<Object>& parent) : Renderable(parent){}

		ParticleSystem::~ParticleSystem()
		{
			particles.reset();

		}


	}
}
