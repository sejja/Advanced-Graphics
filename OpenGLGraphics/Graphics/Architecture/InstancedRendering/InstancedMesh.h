#include "glm.hpp"
#include <glew.h>
#include <iostream>
#include <vector>
#include "Core/ECSystem/Object.h"

#ifndef _INSTANCEDMESH_
#define _INSTANCEDMESH_

namespace Graphics {
	namespace Architecture {
		namespace InstancedRendering {
			class InstancedMesh {
		
			public:
				InstancedMesh();
				~InstancedMesh();


				void drawInstanced(std::shared_ptr<std::vector<std::shared_ptr<Core::Object>>> instancedObject) const;
				void updateTransforms(std::shared_ptr<std::vector<std::shared_ptr<Core::Object>>> instancedObjects);
				std::vector<glm::mat4>* getTransforms_ptr(); 

				bool initiated = false;

			private:
				GLuint VAO, VBO;
				std::vector<glm::mat4> transforms;
			};
		}
	}
}

#endif