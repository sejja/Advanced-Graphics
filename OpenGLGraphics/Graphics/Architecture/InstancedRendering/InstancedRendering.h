#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include "../../Graphics/Primitives/GLBModel.h"
#include "InstancedMesh.h"
#include "InstancedMeshMap.h"
#include "Core/Singleton.h"
#ifndef __InstanceRenderer__
#define __InstanceRenderer__

namespace Graphics {
	namespace Architecture {
		namespace InstancedRendering {

			/// <summary> This Class is a Singleton that cointains models that its meshes oriented to instanced rendering
			/// 
			/// </summary>
			class InstanceRenderer {
			public:
				InstanceRenderer();
				~InstanceRenderer();
			private:
				/*This is a hasmap where key is a weak pointer to a GLB Model and whose value is instanced mesh, different models can point to the same mesh */
				std::unordered_map < std::weak_ptr<Graphics::Primitives::GLBModel >, Graphics::Architecture::InstancedRendering::InstancedMesh > ptr_GLBModel_HashMap;
				InstancedMeshMap meshMap;
			};
		}
	}
}

#endif // !