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
				std::shared_ptr<std::vector<std::shared_ptr<Core::Object>>> objects;
				std::shared_ptr<InstancedMesh> instancedMesh;
			}InstancedRenderNode;

			/// <summary>
			/// This dataStructures is a singleton that determine if the drawing geometry is instanced or not
			/// </summary>
			class InstancedMeshMap {
			public:
				void put(Graphics::Primitives::Mesh* key, std::weak_ptr<Core::Object> value);
				std::weak_ptr<InstancedMesh> get(std::weak_ptr<Graphics::Primitives::Mesh> key);
				void fetch_Instanced();
				void initInstancedMesh(InstancedMesh* instancedMesh);
				InstancedRenderNode CreateRenderNode(std::weak_ptr<Core::Object> value);
				std::unordered_map< Graphics::Primitives::Mesh*, InstancedRenderNode >* getInstancedMap();
				void InstancedRender();

				void printInstancedMap();
				void printMeshMap();
			private:
				/* This map link a mesh to every object that has this mesh  */
				std::unordered_map< std::shared_ptr<InstancedMesh>, std::shared_ptr<std::vector<std::shared_ptr<Core::Object>>> > ptr_InstancedMesh_Objects_Map;

				/* This map link a mesh to every instanced Instanced render node where there is a vector of each object that is rendering in the instanced mesh, in case of contai only one object the instanced rendering is not initiated */
				std::unordered_map< Graphics::Primitives::Mesh* , InstancedRenderNode > ptr_Mesh_Objetcs_Map;
			};
		}
	}
}

#endif