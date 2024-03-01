#ifndef _PARTICLESYSTEM_
#define _PARTICLESYSTEM_

#include "glm.hpp"
#include "Core/ECSystem/Object.h"
#include "Core/ECSystem/Component.h"
#include "Graphics/Primitives/Renderables.h"
#include <iostream>

namespace Core
{
	namespace Particles
	{
		struct Particle
		{
			glm::vec3 pos;
			glm::vec2 size;
			glm::vec4 color;
			unsigned int lifeTime = 0;
		};

		class ParticleSystem : public Core::Graphics::Renderable
		{
			public:
				glm::vec3 center;
				glm::vec3 velocity;
				glm::vec3 acceleration;

				ParticleSystem(const std::weak_ptr<Object>& parent);
				~ParticleSystem();

				void Update() override;


				virtual void Render() const override {
					std::cout << "Unimplemented methdod Render of Default Particle System \n";
				};

			protected:
				std::vector<Particle> particles;
				int ParticleFunction(Particle *p_particle);
		};
	}
}

#endif // !_PARTICLESYSTEM_
