//
//	SSAONoise.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "SSAONoise.h"
#include <glm.hpp>
#include <vector>
#include <random>

namespace Graphics {
	namespace Architecture {
		namespace SSAO {
			SSAONoise::SSAONoise()
			{
			}

			void SSAONoise::HemisphereNoise() {
				std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
				std::default_random_engine generator;

				std::vector<glm::vec3> ssaoNoise;

				for (unsigned int i = 0; i < 16; i++) {
					glm::vec3 noise(
						randomFloats(generator) * 2.0 - 1.0,
						randomFloats(generator) * 2.0 - 1.0,
						0.0f);
					ssaoNoise.push_back(noise);
				}
			}
		}
	}
}