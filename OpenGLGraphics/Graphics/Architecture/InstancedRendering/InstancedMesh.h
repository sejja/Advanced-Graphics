#include "glm.hpp"
#include <glew.h>
#include <iostream>
#include <vector>
#include "Core/ECSystem/Object.h"
#include "../../Graphics/Primitives/GLBMesh.h"

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
				void setAssociatedMesh(Graphics::Primitives::Mesh* mesh);

				std::vector<glm::mat4>* getTransforms_ptr(); 

				bool initiated = false;

			private:
				Core::Graphics::ShaderProgram* shaderProgram = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/InstancedShaders/InstancedGeometry.shader").get()->Get();
				GLuint ubo;
				std::vector<glm::mat4> transforms;
				std::shared_ptr<Graphics::Primitives::Mesh> asociatedMesh;
			};
		}
	}
}

#endif