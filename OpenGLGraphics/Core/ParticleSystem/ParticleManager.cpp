#include "ParticleManager.h"
#include <iostream>
#include "ParticleSystem.h"

namespace Core
{
	namespace Particles
	{
		ParticleMangager::ParticleMangager() {
			SetName("ParticleManager");
			SetID("PARTICLE_MANAGER");
		}
		ParticleMangager::~ParticleMangager() {}

		/*Add a ParticleSystem as a component*/
		void ParticleMangager::AddComponent(std::shared_ptr<Core::Component>&& component)
		{
			this->mComponents.push_back(component);
		}

		/* This functions returns a raw pointer of the vector of particleSystems
		*	-> REMENBER THAT THE CAST
		*/
		std::vector<std::shared_ptr<Component>>* ParticleMangager::GetParticleSystems()
		{
			return &mComponents;
		}

		/*Add a ParticleSystem as a component*/
		void ParticleMangager::Render(Core::Primitives::Camera* camera)
		{
			this->camera = camera;
			std::for_each(mComponents.begin(), mComponents.end(), [this, camera](std::shared_ptr<Core::Component> component)
				{
					std::shared_ptr<Core::Particles::ParticleSystem> system = std::dynamic_pointer_cast<Core::Particles::ParticleSystem>(component);
					system.get()->SetCameraReference(camera);
					system->Render();
				}
			);
		}

		void ParticleMangager::loadParticles(char* path)
		{

		}

		void ParticleMangager::saveParticles(char* path)
		{

		}
	}
}