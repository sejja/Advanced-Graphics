//
//	SSAONoise.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "SSAONoise.h"
#include <random>

namespace Graphics {
	namespace Architecture {
		namespace SSAO {
			// ------------------------------------------------------------------------
			/*! Default Constructor
			*
			*  Gnerates a granular hemisphere texture for sampling oclussion
			*/ //----------------------------------------------------------------------
			SSAONoise::SSAONoise() {
				std::vector<glm::vec3> ssaoNoise = HemisphereNoise();

				glGenTextures(1, &mNoiseTexture);
				glBindTexture(GL_TEXTURE_2D, mNoiseTexture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			// ------------------------------------------------------------------------
			/*! Hemisphere Noise
			*
			*  Creates the Noise distributed that forms a hemisphere (north), without weights
			*/ //----------------------------------------------------------------------
			std::vector<glm::vec3> SSAONoise::HemisphereNoise() {
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

				return ssaoNoise;
			}
		}
	}
}