#ifndef __SCENE_VIEW_H__
#define __SCENE_VIEW_H__

#include <iostream>
#include "Dependencies/ImGui/imgui.h"
#include "Graphics/OpenGLPipeline.h"
#include "Core/Editor/mouse_picking.h"


class SceneView {
public:
    void Render(Core::Graphics::OpenGLPipeline& pipeline);
	bool isHovered();

private:
    MousePicking mousePicking;

    bool isWHovered = false;
};


#endif 
