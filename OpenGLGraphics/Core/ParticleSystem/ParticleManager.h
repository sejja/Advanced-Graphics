#ifndef _PARTICLEMANAGER_
#define _PARTICLEMANAGER_

#include "glm.hpp"
#include "Core/ECSystem/Object.h"
#include "Graphics/Camera.h"

namespace Core
{
	namespace Particles 
	{
		class ParticleMangager : public Core::Object
		{
		public:
			ParticleMangager();
			~ParticleMangager();

			void AddComponent(std::shared_ptr<Core::Component>&& component);
			std::vector<std::shared_ptr<Component>>* GetParticleSystems();
			void Render(Core::Primitives::Camera* camera);
		private:
			Core::Primitives::Camera* camera;
		};
	}
}
#endif 

