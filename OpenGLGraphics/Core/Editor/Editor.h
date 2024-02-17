#ifndef __EDITOR__H__
#define __EDITOR__H__

#include <unordered_map>
#include <vector>
#include <memory>
#include "Dependencies/ImGui/imgui.h"
#include "Graphics/OpenGLPipeline.h"
#include "Interface/Properties.h"


class Editor {
public:
    Editor();

    void Render(Core::Graphics::OpenGLPipeline& pipeline);

    bool IsEditorLocked();

private:
    bool editorLocked;
    Properties properties;


};


#endif 