#include "Pipeline.h"

void Core::Pipeline::AddRenderable(std::weak_ptr<Renderable> x) {
	mGroupedRenderables.emplace(x.lock());
}
