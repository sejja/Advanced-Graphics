#include "ParticleManager.h"
#include <iostream>
#include "Graphics/Primitives/Renderables.h"

namespace Core
{
	namespace Particles
	{
		ParticleManager::ParticleManager(){}
		ParticleManager::~ParticleManager() {}

		void ParticleManager::AddComponent(std::shared_ptr<Core::Particles::ParticleSystem>&& component)
		{
			this->mComponents.push_back(component);
		}
		/* This functions returns a raw pointer of the vector of particleSystems
		*/
		std::vector<std::shared_ptr<Component>>* ParticleManager::GetParticleSystems()
		{
			return &mComponents;
		}

		void ParticleManager::Render(Core::Primitives::Camera* camera)
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
