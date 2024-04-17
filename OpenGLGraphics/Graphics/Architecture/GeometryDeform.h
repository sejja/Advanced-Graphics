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
		public:
			GeometryDeform();
			~GeometryDeform();
			void DecalPass(const GBuffer& gbuffer);

			static std::vector<Decal*> mDecals;
		};
	}
}

#endif