//
//	Kernel.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef _KERNEL__H_
#define _KERNEL__H_

#include <vector>
#include <glm.hpp>

namespace Graphics {
	namespace Architecture {
		namespace SSAO {
			class Kernel {
				static std::vector<glm::vec3> SSAOKernel();
			};
		}
	}
}

#endif