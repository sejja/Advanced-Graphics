#ifndef __EDITOR__H__
#define __EDITOR__H__

#include <unordered_map>
#include <vector>
#include <memory>
#include "Dependencies/ImGui/imgui.h"
#include "Graphics/OpenGLPipeline.h" 


class Editor {
public:
    Editor();

    void render(Core::Graphics::OpenGLPipeline& pipeline);

    bool isEditorLocked();

private:
    bool editorLocked;


};


#endif 