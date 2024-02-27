#include "Core/ParticleSystem/ParticleSystem.h"
#include <iostream>
namespace Core
{
	namespace Particles
	{
		ParticleSystem::ParticleSystem(const std::weak_ptr<Object>& parent) : Renderable(parent){}

		ParticleSystem::~ParticleSystem()
		{
		}

		void ParticleSystem::Update()
		{
			std::for_each(particles.begin(), particles.end(), [this](Core::Particles::Particle& particle) {
				ParticleFunction(&particle);
			});
		}
		int ParticleSystem::ParticleFunction(Particle* p_particle)
		{
			std::cout << "Unimplemented methdod of Default Particle System" << std::endl;
			return -1;
		}
	}
}
