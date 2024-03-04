#include "ParticleManager.h"
#include <iostream>
#include "ParticleSystem.h"

namespace Core
{
	namespace Particles
	{
		ParticleMangager::ParticleMangager() {}
		ParticleMangager::~ParticleMangager() {}

		void ParticleMangager::AddComponent(std::shared_ptr<Core::Component>&& component)
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
			this->camera = camera;
			std::for_each(mComponents.begin(), mComponents.end(), [this, camera](std::shared_ptr<Core::Component> component)
				{
					//std::cout << "Render particles... \n";
					std::shared_ptr<Core::Particles::ParticleSystem> system = std::dynamic_pointer_cast<Core::Particles::ParticleSystem>(component);
					system.get()->SetCameraReference(camera);
					system->Render();
				}
			);
		}
	}
}