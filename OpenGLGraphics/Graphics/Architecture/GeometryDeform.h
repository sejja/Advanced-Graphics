//
//	GeometryDeform.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 19/04/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _GEOMETRY_DEFORM__H_
#define _GEOMETRY_DEFORM__H_

#include <vector>
#include <memory>
#include "Graphics/Primitives/Decal.h"
#include "Graphics/Architecture/GBuffer.h"

namespace Graphics {
	namespace Architecture {
		class GeometryDeform {
		#pragma region //Constructor & Destructor
		public:
			GeometryDeform();
			~GeometryDeform();
		#pragma endregion

		#pragma region //Methods
			void DecalPass(const GBuffer& gbuffer);
			static void AddDecal(Primitives::Decal* decal);
			static void RemoveDecal(Primitives::Decal* decal);
		#pragma endregion

		#pragma region //Members
		private:
			static std::vector<Primitives::Decal*> mDecals;
			Core::Assets::Asset<Primitives::GLBModel> mModel;
			Core::Assets::Asset<Core::Graphics::ShaderProgram> mShader;
		#pragma endregion
		};
	}
}

#endif