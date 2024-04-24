#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include "../../Graphics/Primitives/GLBModel.h"
#include "../../Graphics/Primitives/Renderables.h"
#include "../../Core/Graphics/Pipeline.h"
#include "InstancedMesh.h"
#include "InstancedMeshMap.h"
#include "Core/Singleton.h"
#ifndef __InstanceRenderer__
#define __InstanceRenderer__

namespace Graphics {
	namespace Architecture {
		namespace InstancedRendering {

			/// <summary> This Class is a Singleton that cointains models that its meshes oriented to instanced rendering
			/// Si quieres entenderlo animo
			/// </summary>
			class InstanceRenderer {
			public:
				InstanceRenderer();
				~InstanceRenderer();

				void add_To_InstancedRendering(std::weak_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL> > renderer, std::weak_ptr<Core::Object> object);
				void render( int fetch = 0 );
				int is_Instanced(Graphics::Primitives::Mesh* mesh);
				int removeObject(std::weak_ptr<Core::Object> object);
				void fetch();

			private:
				/*This is a hasmap where key is a weak pointer to a GLB Model and whose value is instanced mesh, different models can point to the same mesh */
				//std::unordered_map < std::shared_ptr<Graphics::Primitives::GLBModel >, Graphics::Architecture::InstancedRendering::InstancedMesh > ptr_GLBModel_HashMap;

				/*  It is a raw pointer to a Singleton */
				InstancedMeshMap* meshMap;
			};
		}
	}
}

#endif