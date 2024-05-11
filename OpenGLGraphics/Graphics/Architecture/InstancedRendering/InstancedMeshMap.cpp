#include "InstancedMeshMap.h"
#include <iostream>

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
			void InstancedMeshMap::put(Graphics::Primitives::Mesh* key, std::weak_ptr<Core::Object> value)
			{
				if (this->ptr_Mesh_Objetcs_Map.find(key) != this->ptr_Mesh_Objetcs_Map.end()) this->ptr_Mesh_Objetcs_Map[key].objects.get()->push_back(value.lock());
				else  this->ptr_Mesh_Objetcs_Map[key] = CreateRenderNode(value);
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
				
				return this->ptr_Mesh_Objetcs_Map[key.lock().get()].instancedMesh;
				
			}

			/// <summary>
			/// This function updates the position and new Objects to the instanced rendering. 
			/// </summary>
			void InstancedMeshMap::fetch_Instanced()
			{
				for (const auto& mesh : this->ptr_Mesh_Objetcs_Map)
				{
					if (mesh.second.objects.get()->size() > 1)
					{
						if (!mesh.second.instancedMesh.get()->initiated)
						{
							this->initInstancedMesh(mesh.second.instancedMesh.get(), mesh.first);
						}
						this->ptr_InstancedMesh_Objects_Map[mesh.second.instancedMesh] = mesh.second.objects;
						mesh.second.instancedMesh.get()->updateTransforms(mesh.second.objects);
					}
					else
					{
						this->ptr_InstancedMesh_Objects_Map.erase(mesh.second.instancedMesh);
						mesh.second.instancedMesh.get()->initiated = false;
					}
				}
			}

			void InstancedMeshMap::initInstancedMesh(InstancedMesh* instancedMesh, Graphics::Primitives::Mesh* mesh) {
				instancedMesh->initiated = true;
				instancedMesh->setAssociatedMesh(mesh);
			}

			void InstancedMeshMap::printInstancedMap()
			{
				using namespace std; // Diego Revilla dont kill me pls

				for (const auto& pair : this->ptr_InstancedMesh_Objects_Map)
				{
					cout << "InstancedMesh = " << pair.first << ": \n";
					for_each(pair.second.get()->begin(), pair.second.get()->end(), [](const std::weak_ptr<Core::Object> ptr_object) {
						cout << "\t Object: " << ptr_object.lock() << " | ID : " << ptr_object.lock().get()->GetID() << " | Name : " << ptr_object.lock().get()->GetName() << "\n";
					});
				}
				cout << "Map size: " << this->ptr_InstancedMesh_Objects_Map.size() << std::endl;
			}

			void InstancedMeshMap::printMeshMap()
			{
				using namespace std;
				for (const auto& pair : this->ptr_Mesh_Objetcs_Map)
				{
					cout << "Mesh = " << pair.first << ": \n";
					for_each(pair.second.objects->begin(), pair.second.objects->end(), [](const std::weak_ptr<Core::Object> ptr_object) {
						cout << "\t Object: " << ptr_object.lock() << " | ID : " << ptr_object.lock().get()->GetID() << " | Name : " << ptr_object.lock().get()->GetName() << "\n";
					});
					cout << "Size: " << pair.second.objects->size() << "\n";
					cout << "Instanced: " << pair.second.instancedMesh->initiated << "\n";
				}
				cout << "Map size: " << this->ptr_Mesh_Objetcs_Map.size() << std::endl;
			}

			std::unordered_map< Graphics::Primitives::Mesh*, InstancedRenderNode >* InstancedMeshMap::getInstancedMap()
			{
				return  &(this->ptr_Mesh_Objetcs_Map);
			}

			void InstancedMeshMap::InstancedRender()
			{
				for (const auto& pair : this->ptr_InstancedMesh_Objects_Map)
				{
					pair.first->drawInstanced( pair.second );
				}
			}

			void InstancedMeshMap::removeObject(Core::Object* ptr_obj)
			{

				for (const auto& mesh : this->ptr_Mesh_Objetcs_Map)
				{
					if (mesh.second.objects.get()->size() > 1)
					{
						mesh.second.objects.get()->erase(std::remove_if(mesh.second.objects.get()->begin(), mesh.second.objects.get()->end(), 
							[ptr_obj](std::shared_ptr<Core::Object> x ) {
							return x.get() == ptr_obj;
							}), mesh.second.objects.get()->end());
					}
				}

			}

			void InstancedMeshMap::clear()
			{
				this->ptr_InstancedMesh_Objects_Map.clear();
				this->ptr_Mesh_Objetcs_Map.clear();
			}
		}
	}
}