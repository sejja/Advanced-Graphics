#include "InstancedMeshMap.h"

namespace Graphics {
	namespace Architecture {
		namespace InstancedRendering {

			/**
			 * @brief Initializes the entry for the given key if it does not exist, and adds the value to the vector associated with the key.
			 *
			 * This method ensures that each mesh instance (key) has a corresponding vector of objects (values). If the mesh instance key is already
			 * present in the map, the object is added to the existing vector. If the key is not present, a new vector is created for that key,
			 * and the object is added to this newly created vector.
			 *
			 * @param key A weak pointer to an InstancedRendering::InstancedMesh, used as the key in the map.
			 * @param value A weak pointer to a Core::Object, which is the value to be added to the vector associated with the key.
			 */
			void InstancedMeshMap::put(std::weak_ptr<Graphics::Primitives::Mesh> key, std::weak_ptr<Core::Object> value)
			{
				
				if (this->ptr_Mesh_Objetcs_Map.find(key.lock()) != this->ptr_Mesh_Objetcs_Map.end()) this->ptr_Mesh_Objetcs_Map[key.lock()].objects.get()->push_back(value.lock());
				else  this->ptr_Mesh_Objetcs_Map[key.lock()] = CreateRenderNode(value);
				
			}
			
			InstancedRenderNode InstancedMeshMap::CreateRenderNode(std::weak_ptr<Core::Object> value)
			{
				auto objectVector = std::make_shared<std::vector<std::shared_ptr<Core::Object>>>();
				if (auto lockedValue = value.lock())  objectVector->push_back(lockedValue);
				auto mesh = std::make_shared<Graphics::Architecture::InstancedRendering::InstancedMesh>();

				InstancedRenderNode node;
				node.objects = objectVector;  
				node.instancedMesh = mesh;  

				return node;
			}

			std::weak_ptr<Graphics::Architecture::InstancedRendering::InstancedMesh>  InstancedMeshMap::get(std::weak_ptr<Graphics::Primitives::Mesh> key)
			{
				
				return this->ptr_Mesh_Objetcs_Map[key.lock()].instancedMesh;
				
			}

			void InstancedMeshMap::fetch_Instanced()
			{

				for (const auto& mesh : this->ptr_Mesh_Objetcs_Map)
				{
					if (mesh.second.objects.get()->size() > 1)
					{
						if (!mesh.second.instancedMesh.get()->initiated)
						{
							this->initInstancedMesh(mesh.second.instancedMesh.get()); //Just dont do that
						}
						this->ptr_InstancedMesh_Objects_Map[mesh.second.instancedMesh] = mesh.second.objects;
					}
					else
					{
						this->ptr_InstancedMesh_Objects_Map.erase(mesh.second.instancedMesh);
						mesh.second.instancedMesh.get()->initiated = false;
					}
				}

			}

			void InstancedMeshMap::initInstancedMesh(InstancedMesh* instancedMesh) {
				instancedMesh->initiated = true;
			}
		}
	}
}