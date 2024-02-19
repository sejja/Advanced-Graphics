#include "glm.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Core/ECSystem/Component.h"

#ifndef PARTICLES_YSTEM
#define PARTICLES_YSTEM

namespace Graphics 
{
	namespace Particles // Soy autista
	{
		struct Particle {
			glm::vec3 position;
			glm::vec3 velocity;
			glm::vec4 color;
			float lifetime;
			bool active;
		};

		class ParticleSystem
		{
		public:
			ParticleSystem();
			~ParticleSystem();

			void Update();
			void Render();
		private:
			//std::vector<Particle> particles;
			unsigned int maxParticles;

			void initializeParticles();
			void respawnParticle(Particle& particle);

			Core::Graphics::ShaderProgram ShaderProgram;
			GLuint VAO;
		};
	}
}

#endif 

