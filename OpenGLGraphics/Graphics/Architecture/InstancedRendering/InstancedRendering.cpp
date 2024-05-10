#include<iostream>
#include "InstancedRendering.h"

namespace Graphics::Architecture::InstancedRendering
{
	InstanceRenderer::InstanceRenderer()
	{
		this->meshMap = &Singleton<InstancedMeshMap>::Instance();
		this->shaderProgram = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/InstancedShaders/InstancedGeometry.shader");
	}

	InstanceRenderer::~InstanceRenderer()
	{

	}

	void InstanceRenderer::add_To_InstancedRendering(std::weak_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL> > renderer, std::weak_ptr<Core::Object> object)
	{
		std::vector<Graphics::Primitives::Mesh>& meshes =  renderer.lock().get()->getModel()->getMeshes();
		//std::cout << "Adding Object: " << object.lock() << "\n";
		InstancedMeshMap& map = *this->meshMap;
		std::for_each(meshes.begin(), meshes.end(), [this, &object, &map](Graphics::Primitives::Mesh& x) {
			map.put(&x, object);
			//std::cout << "\tMesh: " << &x << "\n";
		});
		//std::cout << "Size: " << meshes->size() << "\n";
	}

	void InstanceRenderer::render(int fetch)
	{

		if (fetch) this->meshMap->fetch_Instanced();

		shaderProgram->Get()->Bind();
		this->meshMap->InstancedRender();


	}

	void InstanceRenderer::render_shader(int fetch, Core::Graphics::ShaderProgram* shader)
	{
		if (fetch) this->meshMap->fetch_Instanced();

		shader->Bind();
		this->meshMap->InstancedRender();
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

	int InstanceRenderer::has_instanced()
	{
		return this->meshMap->getInstancedMap()->size() > 0;
	}

	int InstanceRenderer::removeObject(std::weak_ptr<Core::Object> object)
	{
		std::cout << "ERROR: UNIMPLEMENTED FUNCTION removeObject\n";
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

	void InstanceRenderer::clear()
	{
		this->meshMap->clear();
	}


}