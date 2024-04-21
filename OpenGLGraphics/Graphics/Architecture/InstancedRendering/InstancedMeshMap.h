#include <unordered_map>
#include <iostream>
#include "InstancedMesh.h"
#include "../../Graphics/Primitives/GLBMesh.h"
#include "Core/ECSystem/Object.h"

#ifndef _INSTANCEDMESHMAP_
#define _INSTANCEDMESHMAP_

namespace Graphics {
	namespace Architecture {
		namespace InstancedRendering {

			typedef struct InstancedRenderNode
			{
				std::weak_ptr<std::vector<std::weak_ptr<Core::Object>>> objects;
				std::weak_ptr<InstancedMesh> instancedMesh;
			}InstancedRenderNode;

			/// <summary>
			/// This dataStructures is a singleton that determine if the drawing geometry is instanced or not
			/// </summary>
			class InstancedMeshMap {
			public:
				void put(std::weak_ptr<Graphics::Primitives::Mesh> key, std::weak_ptr<Core::Object> value);
				std::weak_ptr<InstancedMesh> get(std::weak_ptr<Graphics::Primitives::Mesh> key);
				void fetch_Instanced();
				void initInstancedMesh(InstancedMesh* instancedMesh);
				InstancedRenderNode CreateRenderNode(std::weak_ptr<Core::Object> value);
			private:
				std::unordered_map< std::weak_ptr<InstancedMesh>, std::weak_ptr<std::vector<std::weak_ptr<Core::Object>>> > ptr_InstancedMesh_Objects_Map;
				std::unordered_map< std::weak_ptr<Graphics::Primitives::Mesh>, InstancedRenderNode > ptr_Mesh_Objetcs_Map;
			};
		}
	}
}

#endif