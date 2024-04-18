//
//	GeometryDeform.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 19/04/24
//	Copyright © 2024. All Rights reserved
//

#include "GeometryDeform.h"

namespace Graphics {
	namespace Architecture {
		std::vector<Primitives::Decal*> GeometryDeform::mDecals;

		GeometryDeform::GeometryDeform()
			{}

		GeometryDeform::~GeometryDeform() {}

		void GeometryDeform::DecalPass(const GBuffer& gbuffer) {
			mModel = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Graphics::Primitives::GLBModel>("Content/Meshes/cube_averaged.obj");
			mShader = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/GeometryDecal.shader");
			mShader->Get()->Bind();
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, gbuffer.GetDepthTextureHandle());
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gbuffer.GetNormalTextureHandle());

			for (auto& x : mDecals) {
				x->Render(*mShader->Get());
				mModel->Get()->Draw();
			}
		}
		void GeometryDeform::AddDecal(Primitives::Decal* decal) {
			mDecals.push_back(decal);
		}

		void GeometryDeform::RemoveDecal(Primitives::Decal* decal) {
			mDecals.erase(std::find(mDecals.begin(), mDecals.end(), decal));
		}
	}
}