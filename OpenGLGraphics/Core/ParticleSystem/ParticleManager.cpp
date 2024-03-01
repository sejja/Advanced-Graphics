#include "ParticleManager.h"
#include <iostream>

namespace Core
{
	namespace Particles
	{
		ParticleMangager::ParticleMangager() {}
		ParticleMangager::~ParticleMangager() {}

		void ParticleMangager::AddComponent(std::shared_ptr<Core::Particles::ParticleSystem>&& component)
		{
			this->mComponents.push_back(component);
		}
		/* This functions returns a raw pointer of the vector of particleSystems
		*/
		std::vector<std::shared_ptr<Component>>* ParticleMangager::GetParticleSystems()
		{
			return &mComponents;
		}

		void ParticleMangager::Render(Core::Primitives::Camera* camera)
		{
			std::for_each(mComponents.begin(), mComponents.end(), [this](std::shared_ptr<Core::Component> component)
				{
					std::cout << "Render particles... \n";
					std::shared_ptr<ParticleSystem> system = std::dynamic_pointer_cast<ParticleSystem>(component);
					system->Render();
				}
			);
		}
	}
}