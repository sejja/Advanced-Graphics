#ifndef __DEBUG_TOOLS__H__
#define __DEBUG_TOOLS__H__

#include "Dependencies/ImGui/imgui.h"
#include "Graphics/OpenGLPipeline.h"


class DebugTools {
public:
	static void RenderShadowMapping();

private:
	Core::Graphics::OpenGLPipeline& pipeline;


};

#endif
