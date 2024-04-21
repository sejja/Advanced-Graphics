#include "glm.hpp"
#include <glew.h>

#ifndef _INSTANCEDMESH_
#define _INSTANCEDMESH_

namespace Graphics {
	namespace Architecture {
		namespace InstancedRendering {
			class InstancedMesh {
		
			public:
				InstancedMesh();
				~InstancedMesh();

				bool initiated = false;

			private:
				GLuint VAO, VBO;
			};
		}
	}
}

#endif