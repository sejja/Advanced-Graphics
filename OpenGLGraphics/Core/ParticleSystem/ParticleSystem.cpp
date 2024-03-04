#include "ParticleSystem.h"
#include <iostream>
namespace Core
{
	namespace Particles
	{
		ParticleSystem::ParticleSystem(const std::weak_ptr<Object>& parent) : Renderable(parent){
            /*
            float cubeVertices[] = {
                // Posiciones           // Colores
                -1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f, // Cara trasera
                 1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
                -1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
                -1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f,

                -1.0f, -1.0f,  1.0f,    0.0f, 1.0f, 0.0f, // Cara frontal
                 1.0f, -1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
                 1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
                 1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
                -1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
                -1.0f, -1.0f,  1.0f,    0.0f, 1.0f, 0.0f,

                -1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f, // Cara superior
                -1.0f,  1.0f, -1.0f,    0.0f, 0.0f, 1.0f,
                 1.0f,  1.0f, -1.0f,    0.0f, 0.0f, 1.0f,
                 1.0f,  1.0f, -1.0f,    0.0f, 0.0f, 1.0f,
                 1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
                -1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,

                -1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 0.0f, // Cara inferior
                -1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 0.0f,
                 1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 0.0f,
                 1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 0.0f,
                 1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 0.0f,
                -1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 0.0f,

                -1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 1.0f, // Cara izquierda
                -1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 1.0f,
                -1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 1.0f,
                -1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 1.0f,
                -1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 1.0f,

                 1.0f, -1.0f, -1.0f,    0.0f, 1.0f, 1.0f, // Cara derecha
                 1.0f, -1.0f,  1.0f,    0.0f, 1.0f, 1.0f,
                 1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 1.0f,
                 1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 1.0f,
                 1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, -1.0f,    0.0f, 1.0f, 1.0f,
            };
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            // Vincula el Vertex Array Object primero, luego vincula y configura el VBO
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

            // Atributo de posición
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // Atributo de color
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            */






            
            Particle particles[4] = {
                // Partícula 1
                {
                    glm::vec3(1.0f, 1.0f, 100.0f), // pos
                    glm::vec3(100.0f),       // size
                    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // color (rojo)
                    100                          // lifeTime
                },
                // Partícula 2
                {
                    glm::vec3(1.0f, 0.0f, 100.0f), // pos
                    glm::vec3(100.5f),       // size
                    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // color (verde)
                    200                          // lifeTime
                },
                // Partícula 3
                {
                    glm::vec3(-1.0f, 1.0f, 100.0f),// pos
                    glm::vec3(100.75f),     // size
                    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // color (azul)
                    150                          // lifeTime
                },
                // Partícula 4
                {
                    glm::vec3(1.f, -1.0f, 100.0f),// pos
                    glm::vec3(100.2f),      // size
                    glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // color (amarillo)
                    250                          // lifeTime
                }
            };

            for (int i = 0; i < sizeof(particles) / sizeof(Particle); i++) {
                this->particles.push_back(particles[i]);
            }
            unsigned int VBO, VAO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, this->particles.size() * sizeof(Particle), particles, GL_DYNAMIC_DRAW);

            // Posición
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, pos));

            glBindVertexArray(0);

            shaderProgram = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/Particle.shader");
            #pragma endregion
           
		}

		ParticleSystem::~ParticleSystem()
		{
		}

        void ParticleSystem::SetCameraReference(Core::Primitives::Camera* camera)
        {
            this->camera = camera;
        }

        Core::Primitives::Camera* ParticleSystem::GetCameraReference()
        {
            return camera;
        }

        void ParticleSystem::Update()
		{
			std::for_each(particles.begin(), particles.end(), [this](Core::Particles::Particle& particle) {
				ParticleFunction(&particle);
			});
		}

		int ParticleSystem::ParticleFunction(Particle* p_particle)
		{
            //std::cout << "Unimplemented methdod ParticleFunction of Default Particle System \n";
			return -1;
		}
	}
}
