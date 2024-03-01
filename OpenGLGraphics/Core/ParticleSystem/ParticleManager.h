#ifndef _PARTICLEMANAGER_
#define _PARTICLEMANAGER_

#include "glm.hpp"
#include "Core/ECSystem/Object.h"
#include "Core/ParticleSystem/ParticleSystem.h"
#include "Graphics/Camera.h"

namespace Core
{
	namespace Particles 
	{
		class ParticleManager : public Core::Object
		{
		public:
			ParticleManager();
			~ParticleManager();

			void AddComponent(std::shared_ptr<Core::Particles::ParticleSystem>&& component);
			std::vector<std::shared_ptr<Component>>* GetParticleSystems();
			void Render(Core::Primitives::Camera* camera);
		};
	}
}
#endif 

