#ifndef _PARTICLESYSTEM_
#define _PARTICLESYSTEM_

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Core/ECSystem/Object.h"
#include "Core/ECSystem/Component.h"
#include "Graphics/Primitives/Renderables.h"
#include "Graphics/Camera.h"
#include <iostream>

namespace Core
{
	namespace Particles
	{
		struct Particle
		{
			glm::vec3 pos;
			glm::vec3 velocity;
		};

		class ParticleSystem : public Core::Graphics::Renderable
		{
			public:
				glm::vec3 center = glm::vec3(0.0f, 0.0f, 40.0f);
				glm::vec3 velocity;
				glm::vec3 acceleration;

				ParticleSystem(const std::weak_ptr<Object>& parent);
				~ParticleSystem();
				
				void SetCameraReference(Core::Primitives::Camera* camera);
				Core::Primitives::Camera* GetCameraReference();
				void Update() override;

				virtual void Render() const override {

					//Bind shader program
					shaderProgram->Get()->Bind();

					//Get camera projection and view
					glm::mat4 view = camera->GetViewMatrix(); 
					glm::mat4 uProjection = this->projection;

					//Get default color of the system
					glm::vec color = this->basecolor;

					//Set uniforms
					shaderProgram->Get()->SetShaderUniformMatrix4d("view", &view);
					shaderProgram->Get()->SetShaderUniformMatrix4d("projection", &uProjection);
					shaderProgram->Get()->SetShaderUniform("pointSize", this->particleSize);
					shaderProgram->Get()->SetShaderUniform("particleColor", &color);

					//Bind Vertex array
					glBindVertexArray(VAO);

					//Draw all particle in the same instance
					glDrawArraysInstanced(GL_POINTS, 0, 1, this->particles.size());

					//Unbind
					glBindVertexArray(0);
				};

			protected:
				GLuint VAO, VBO;
				Asset<Core::Graphics::ShaderProgram> shaderProgram;
				std::vector<Particle> particles;
				Core::Primitives::Camera* camera;
				glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);

				int ParticleFunction(Particle *p_particle);
				void initTestParticles();
				virtual void init();

			private:
				//Test data
				const unsigned int nParticlesTest = 20;
				const float height = 10.0f;
				const float width = 10.0f;
				const float particleSize = 1.0f;
				const glm::vec4 basecolor = glm::vec4(1.0f, 0.0f, 0.f, 1.0f);
		};
	}
}

#endif // !_PARTICLESYSTEM_
