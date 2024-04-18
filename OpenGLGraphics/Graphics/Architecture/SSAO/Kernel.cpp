//
//	Kernel.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "Kernel.h"
#include <random>

namespace Graphics {
	namespace Architecture {
		namespace SSAO {
			std::vector<glm::vec3> Kernel::SSAOKernel() {
                std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
                std::default_random_engine generator;
                std::vector<glm::vec3> ssaoKernel;
                for (unsigned int i = 0; i < 64; ++i) {
                    glm::vec3 sample(
                        randomFloats(generator) * 2.0 - 1.0,
                        randomFloats(generator) * 2.0 - 1.0,
                        randomFloats(generator)
                    );
                    sample = glm::normalize(sample);
                    sample *= randomFloats(generator);
                    ssaoKernel.push_back(sample);
                }

                return ssaoKernel;
			}
		}
	}
}