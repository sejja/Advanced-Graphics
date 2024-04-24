#include<iostream>
#include "InstancedRendering.h"

namespace Graphics::Architecture::InstancedRendering
{
	InstanceRenderer::InstanceRenderer()
	{
		this->meshMap = &Singleton<InstancedMeshMap>::Instance();
	}

	InstanceRenderer::~InstanceRenderer()
	{

	}

	void InstanceRenderer::add_To_InstancedRendering(std::weak_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL> > renderer, std::weak_ptr<Core::Object> object)
	{
		std::vector<Graphics::Primitives::Mesh>* meshes =  renderer.lock().get()->getModel()->getMeshes();
		//std::cout << "Adding Object: " << object.lock() << "\n";
		InstancedMeshMap& map = *this->meshMap;
		std::for_each(meshes->begin(), meshes->end(), [this, &object, &map](Graphics::Primitives::Mesh& x) {
			map.put(&x, object);
			//std::cout << "\tMesh: " << &x << "\n";
		});
		//std::cout << "Size: " << meshes->size() << "\n";
	}

	void InstanceRenderer::render()
	{
		std::cout << "ERROR: UNIMPLEMENTED FUNCTION render\n";
	}

	int InstanceRenderer::is_Instanced(Graphics::Primitives::Mesh* mesh)
	{
		if (meshMap->getInstancedMap()->find(mesh) == meshMap->getInstancedMap()->end())
		{
			//std::cout << "MESH NO IDENTIFICADO CON RESPECTO AL ARCHIVO INI: " << mesh << "\n";
			return 0;
		}

		return meshMap->getInstancedMap()->at(mesh).instancedMesh.get()->initiated;
	}

	int InstanceRenderer::removeObject(std::weak_ptr<Core::Object> object)
	{
		std::cout << "ERROR: UNIMPLEMENTED FUNCTION is_Instanced\n";
		return -1;
	}

	void InstanceRenderer::fetch()
	{
		std::cout << std::endl;
		//std::cout << "PREFETCH INFO: \n";
		//this->meshMap->printMeshMap();
		//std::cout << std::endl;
		//std::cout << "INSTANCED INFO: \n";
		this->meshMap->fetch_Instanced();
		this->meshMap->printInstancedMap();
		//this->meshMap->printMeshMap();
		std::cout << std::endl;
	}
}