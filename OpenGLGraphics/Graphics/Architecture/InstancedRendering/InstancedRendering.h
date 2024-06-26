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
				void render_shader(int fetch, Core::Graphics::ShaderProgram* shader);
				int is_Instanced(Graphics::Primitives::Mesh* mesh);
				int has_instanced();
				int removeObject(std::weak_ptr<Core::Object> object);
				void fetch();
				void clear();

			private:

				/*  It is a raw pointer to a Singleton */
				InstancedMeshMap* meshMap;
				Core::Assets::Asset<Core::Graphics::ShaderProgram> shaderProgram;
			};
		}
	}
}

#endif