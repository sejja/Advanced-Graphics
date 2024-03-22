//
//	Pipeline.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 22/03/24
//	Copyright © 2024. All Rights reserved
//

#include "Pipeline.h"

void Core::Pipeline::AddRenderable(const std::weak_ptr<Renderable>& x) {
	mGroupedRenderables.emplace_back(x.lock());
}
