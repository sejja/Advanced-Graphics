#include "ParticleManager.h"
#include <iostream>
#include "Graphics/Primitives/Renderables.h"

namespace Core
{
	namespace Particles
	{
		ParticleMangager::ParticleMangager(){}
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
			std::cout << "Particle Manager render... \n";
			std::for_each(mComponents.begin(), mComponents.end(), [this](std::shared_ptr<Component>& component)
				{
					std::shared_ptr<Core::Particles::ParticleSystem> renderable = std::dynamic_pointer_cast<Core::Particles::ParticleSystem>(component);
					renderable.get()->Render();
				}
			);
		}
	}
}
