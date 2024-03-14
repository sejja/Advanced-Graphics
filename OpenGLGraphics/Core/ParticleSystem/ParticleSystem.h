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
		};

		class ParticleSystem : public Core::Graphics::Renderable
		{
			public:

				ParticleSystem(const std::weak_ptr<Object>& parent);
				~ParticleSystem();

				
				void SetCameraReference(Core::Primitives::Camera* camera);
				Core::Primitives::Camera* GetCameraReference();
				void SetSystemCenter(glm::vec3 newCenter);
				glm::vec3 GetSystemCenter();

				float q_rsqrt(float number);
				float RandomFloat(float a, float b);

				void Update() override;
				virtual void Render() const override {

					//Bind shader program
					shaderProgram->Get()->Bind();

					//Get camera projection and view
					glm::mat4 view = camera->GetViewMatrix(); 
					glm::mat4 uProjection = this->projection;

					//New instance of systemProperties
					glm::vec4 color = this->baseColor;
					glm::vec3 systemCenter = this->center;
					glm::vec3 acceleration = this->acceleration;
					float pointSize = this->particleSize;
					float systemSigma = this->sigma;
					float systemHeight = this->height;
					float systemWidth = this->width;
					int systemDelta = this->delta;
					//std::cout << systemDelta << "\n";

					//Set uniforms
					shaderProgram->Get()->SetShaderUniformMatrix4d("view", &view);
					shaderProgram->Get()->SetShaderUniformMatrix4d("projection", &uProjection);
					shaderProgram->Get()->SetShaderUniform("pointSize", pointSize);
					shaderProgram->Get()->SetShaderUniform("particleColor", &color);
					shaderProgram->Get()->SetShaderUniform("center", &systemCenter);
					shaderProgram->Get()->SetShaderUniform("delta", systemDelta);
					shaderProgram->Get()->SetShaderUniform("sigma", &systemSigma);
					shaderProgram->Get()->SetShaderUniform("acceleration", &acceleration);
					shaderProgram->Get()->SetShaderUniform("height", &systemHeight);
					//shaderProgram->Get()->SetShaderUniform("width", &systemWidth);

					//Bind Vertex array
					glBindVertexArray(VAO);

					//Draw all particle in the same instance
					glDrawArraysInstanced(GL_POINTS, 0, 1, this->particles.size());

					//Unbind
					glBindVertexArray(0);
				};

			protected:
				// (0,0,0) by default
				glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
				//(0,0,0) by default
				glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
				// Red by default
				glm::vec4 baseColor = glm::vec4(1.0f, 0.0f, 0.f, 1.0f);
				//10f by default
				float height = 20.0f;
				//10f by default
				float width = 10.0f;
				//1.0f by default
				float particleSize = 1.0f;
				//0.05f by default
				float sigma = 0.05f;
				//Clock, increase with each update
				int delta = 0;

				GLuint VAO, VBO;
				Asset<Core::Graphics::ShaderProgram> shaderProgram;
				std::vector<Particle> particles;
				Core::Primitives::Camera* camera;
				glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);

				int ParticleFunction(Particle *p_particle);
				virtual void Init();
				virtual void InitParticles();
				glm::vec4 NormalizeRGBA(int R, int G, int B, int A);
		};
	}
}

#endif // !_PARTICLESYSTEM_
