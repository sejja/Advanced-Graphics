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
		InstancedMeshMap& map = *this->meshMap;
		std::for_each(meshes.begin(), meshes.end(), [this, &object, &map](Graphics::Primitives::Mesh& x) {
			map.put(&x, object);
		});
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
		try
		{
			this->meshMap->removeObject(object.lock().get());
			this->meshMap->fetch_Instanced();
			return 0;
		}
		catch (const std::exception&)
		{
			return -1;
		}
	}

	void InstanceRenderer::fetch()
	{
		std::cout << std::endl;

		this->meshMap->fetch_Instanced();
		this->meshMap->printInstancedMap();

		std::cout << std::endl;
	}

	void InstanceRenderer::clear()
	{
		this->meshMap->clear();
	}


}