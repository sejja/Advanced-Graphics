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
		std::vector<Decal*> GeometryDeform::mDecals;

		GeometryDeform::GeometryDeform() {}
		GeometryDeform::~GeometryDeform() {}

		void GeometryDeform::DecalPass(const GBuffer& gbuffer) {
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, gbuffer.GetDepthTextureHandle());
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gbuffer.GetNormalTextureHandle());

			for (auto& x : mDecals) {
				x->Render();
			}
		}
	}
}