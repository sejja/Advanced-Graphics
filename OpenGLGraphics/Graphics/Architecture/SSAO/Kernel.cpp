//
//	Kernel.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "Kernel.h"
#include <random>
#include <glew.h>
#include "Core/Math/Lerp.h"

namespace Graphics {
	namespace Architecture {
		namespace SSAO {
            float ourLerp(float a, float b, float f)
            {
                return a + f * (b - a);
            }

            // ------------------------------------------------------------------------
            /*! SSAO Kernel
            *
            *   Generate samples that form a hemisphere
            */ //----------------------------------------------------------------------
			std::vector<glm::vec3> Kernel::SSAOKernel() {
                std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
                std::default_random_engine generator;
                std::vector<glm::vec3> ssaoKernel;

                //generates a set of sample of kerners oriented along the normal of a surface
                for (unsigned int i = 0; i < 64; ++i) {
                    glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
                    sample = glm::normalize(sample);
                    sample *= randomFloats(generator);
                    float scale = float(i) / 64.0f;

                    // scale samples s.t. they're more aligned to center of kernel
                    scale = ourLerp(0.1f, 1.0f, scale * scale);
                    sample *= scale;
                    ssaoKernel.push_back(sample);
                }

                return ssaoKernel;
			}
		}
	}
}