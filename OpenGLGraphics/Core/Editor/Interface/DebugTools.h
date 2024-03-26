#ifndef __DEBUG_TOOLS__H__
#define __DEBUG_TOOLS__H__

#include "Dependencies/ImGui/imgui.h"
#include "Graphics/OpenGLPipeline.h"
#include "Graphics/Tools/FrameBuffer.h"


class DebugTools {
public:
	void RenderShadowMapping(Core::Graphics::OpenGLPipeline& pipeline);
	void RenderDeferredRendering(Core::Graphics::OpenGLPipeline& pipeline);


};

#endif
