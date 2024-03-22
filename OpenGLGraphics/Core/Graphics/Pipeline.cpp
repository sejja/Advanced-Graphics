//
//	Pipeline.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 22/03/24
//	Copyright © 2024. All Rights reserved
//

#include "Pipeline.h"

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Add Renderable
		*
		*   Adds a Renderable to the Pipeline
		*/ //----------------------------------------------------------------------
		void Pipeline::AddRenderable(std::shared_ptr<Renderable>&& x) {
			mGroupedRenderables.emplace_back(std::move(x));
		}
	}
}
